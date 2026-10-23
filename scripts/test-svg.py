#!/usr/bin/env python3
"""SVG regression tests. Set MOLSCRIPT to test another compiled executable."""

import io
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
BINARY = Path(os.environ.get("MOLSCRIPT", ROOT / "molscript")).resolve()
NS = {"s": "http://www.w3.org/2000/svg"}


class SvgTests(unittest.TestCase):
    def run_scene(self, text, success=True):
        with tempfile.TemporaryDirectory(prefix="molscript-svg-") as tmp:
            src = Path(tmp) / "test.in"
            dst = Path(tmp) / "test.svg"
            src.write_text(text)
            proc = subprocess.run(
                [str(BINARY), "-svg", "-in", str(src), "-out", str(dst)],
                capture_output=True, text=True, timeout=30,
            )
            if not success:
                self.assertNotEqual(proc.returncode, 0)
                return proc.stderr
            self.assertEqual(proc.returncode, 0, proc.stderr)
            return ET.parse(dst).getroot()

    def scene(self, body, area="0 0 200 200"):
        return self.run_scene(
            f"plot\narea {area};\nwindow 10;\nslab 20;\n"
            "set segmentsize 100, shading 0;\n" + body + "\nend_plot\n"
        )

    def test_coloured_points(self):
        root = self.scene("object inline;\nPC 2\n-2 0 0 1 0 0\n2 0 0 0 0 1\nQ\n")
        circles = root.findall(".//s:circle", NS)
        self.assertEqual(len(circles), 2)
        self.assertIn("fill:rgb(255,0,0)", circles[0].get("style"))
        self.assertIn("fill:rgb(0,0,255)", circles[1].get("style"))

    def test_short_label_mask(self):
        root = self.scene('set labelmask "g"; label 0 0 0 "abcdefghijk";')
        spans = root.findall(".//s:tspan", NS)
        self.assertEqual([s.text for s in spans], ["a", "bcdefghijk"])
        self.assertEqual(spans[0].get("font-family"), "Symbol")
        self.assertIsNone(spans[1].get("font-family"))

    def test_opacity_applied_once_per_primitive(self):
        root = self.scene("set transparency 0.5; object inline;\nP 1\n0 0 0\nQ\n")
        groups = root.findall('.//s:g[@opacity="0.5"]', NS)
        self.assertEqual(len(groups), 1)
        self.assertEqual(len(groups[0].findall("s:circle", NS)), 1)
        self.assertNotIn("fill-opacity", ET.tostring(root).decode())
        self.assertNotIn("stroke-opacity", ET.tostring(root).decode())

    def test_transparent_triangles_have_no_sealing_stroke(self):
        root = self.scene("set transparency 0.35; object inline;\nT 3\n-2 -2 0\n2 -2 0\n0 2 0\nQ\n")
        polygon = root.find(".//s:polygon", NS)
        self.assertIn("stroke:none", polygon.get("style"))

    def test_fully_transparent_geometry_not_drawn(self):
        root = self.scene("set transparency 1; object inline;\nP 1\n0 0 0\nQ\nlabel 0 0 0 hidden;")
        self.assertEqual(len(root.findall(".//s:circle", NS)), 0)
        self.assertEqual(len(root.findall(".//s:text", NS)), 0)

    def test_small_transparency_preserved(self):
        root = self.scene("set transparency 0.0001; object inline;\nP 1\n0 0 0\nQ\n")
        values = [float(g.get("opacity")) for g in root.findall('.//s:g[@opacity]', NS)]
        self.assertEqual(len(values), 1)
        self.assertAlmostEqual(values[0], 0.9999)

    def test_labelclip_off_really_outside_clip_group(self):
        root = self.scene('set transparency 0.5, labelclip off; label 4.9 0 0 "outside"; '
                          'set labelclip on; label 0 0 0 "inside";')
        clipped = root.find('.//s:g[@clip-path]', NS)
        self.assertEqual(["".join(t.itertext()) for t in clipped.findall('.//s:text', NS)], ["inside"])
        self.assertEqual(len(root.findall('.//s:text', NS)), 2)

    def test_opaque_strokes_and_xml_escaping_preserved(self):
        root = self.scene('label 0 0 0 "A & B < C"; object inline;\nT 3\n-2 -2 0\n2 -2 0\n0 2 0\nQ\n')
        self.assertEqual("".join(root.find('.//s:text', NS).itertext()), "A & B < C")
        self.assertIn("stroke-width:0.35", root.find('.//s:polygon', NS).get('style'))
        self.assertEqual(len(root.findall('.//s:g[@opacity]', NS)), 0)

    def test_negative_plot_area(self):
        root = self.scene("label 0 0 0 test;", area="-100 -100 100 100")
        self.assertEqual([float(x) for x in root.get("viewBox").split()], [-100, -100, 200, 200])

    def test_multiple_plot_areas(self):
        root = self.run_scene("plot area -200 0 -100 100; window 10; label 0 0 0 left; end_plot\n"
                              "plot area 0 0 100 100; window 10; label 0 0 0 right; end_plot\n")
        self.assertEqual([float(x) for x in root.get("viewBox").split()], [-200, 0, 300, 100])
        self.assertEqual(len(root.findall(".//s:clipPath", NS)), 2)

    def test_zero_plot_area_rejected(self):
        error = self.run_scene("plot area 0 0 0 100; window 10; label 0 0 0 test; end_plot", success=False)
        self.assertIn("invalid area", error)

    def test_sphere_highlights_composited_together(self):
        pdb = ROOT / "examples" / "ras.pdb"
        root = self.scene(f'read mol "{pdb}"; transform atom * by centre position in residue 201; '
                          'set transparency 0.5; cpk in residue 201;')
        groups = root.findall('.//s:g[@opacity="0.5"]', NS)
        self.assertTrue(groups)
        self.assertTrue(any(g.find("s:circle", NS) is not None and g.find("s:path", NS) is not None for g in groups))

    def test_raster_opacity_at_point_border(self):
        try:
            import cairosvg
            from PIL import Image
        except ImportError:
            self.skipTest("optional CairoSVG and Pillow required for pixel test")
        body = "set linewidth 10, depthcue 0, linecolour red; object inline;\nP 1\n0 0 0\nQ\n"
        root = self.scene("set transparency 0.5; " + body)
        im = Image.open(io.BytesIO(cairosvg.svg2png(bytestring=ET.tostring(root)))).convert("RGB")
        # The stroke overlaps the filled disk at this pixel. Alpha must apply once.
        red, green, blue = im.getpixel((103, 100))
        self.assertEqual(red, 255)
        self.assertTrue(125 <= green <= 129, (red, green, blue))
        self.assertEqual(green, blue)


if __name__ == "__main__":
    unittest.main(verbosity=2)
