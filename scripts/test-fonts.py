#!/usr/bin/env python3
"""Font selection, state snapshots, safe names and consumer-format regressions."""
import html
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
MOL = str(Path(os.environ.get("MOLSCRIPT", ROOT / "molscript")).resolve())
HELP = subprocess.run([MOL, "-h"], text=True, capture_output=True).stderr
SVG = "{http://www.w3.org/2000/svg}"
SCENE = 'plot window 10; background white; set linecolour black; label 0 0 0 "Wi 2.47 Å²"; end_plot'
MIXED = '''plot window 12;
set labelfont "sans"; label 0 3 0 "outer";
push; set labelfont "monospace"; label 0 0 0 "inner Å²";
pop; label 0 -3 0 "restored";
end_plot'''


class Fonts(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="molscript-fonts-")
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name)

    def render(self, mode, scene=SCENE, args=(), status=0):
        result = subprocess.run([MOL, "-s", mode, *args], input=scene, text=True,
                                capture_output=True, timeout=30, cwd=self.path)
        self.assertEqual(result.returncode, status, result.stderr)
        return result.stdout

    def svg_fonts(self, data):
        return [("".join(e.itertext()), e.get("font-family"))
                for e in ET.fromstring(data).iter(SVG + "text")]

    def test_svg_presets_and_native_names(self):
        for name, family in (("default", "Times-Roman"), ("serif", "serif"),
                             ("sans", "sans-serif"), ("sans-serif", "sans-serif"),
                             ("monospace", "monospace"), ("DejaVu Sans", "DejaVu Sans")):
            data = self.render("-svg", args=("-font", name))
            self.assertEqual(self.svg_fonts(data), [("Wi 2.47 Å²", family)])
        self.assertEqual(self.render("-svg"), self.render("-svg", args=("-font", "default")))

    def test_state_snapshots_and_restore(self):
        self.assertEqual(self.svg_fonts(self.render("-svg", MIXED)),
                         [("outer", "sans-serif"), ("inner Å²", "monospace"), ("restored", "sans-serif")])
        # Depth ordering must not replace a label's captured font with the last state.
        scene = MIXED.replace('0 3 0 "outer"', '0 3 3 "outer"')
        self.assertEqual(dict(self.svg_fonts(self.render("-svg", scene))),
                         {"outer": "sans-serif", "inner Å²": "monospace", "restored": "sans-serif"})

    def test_cli_default_resets_per_plot(self):
        data = self.render("-svg", MIXED + "\n" + SCENE, ("-font", "serif"))
        self.assertEqual(self.svg_fonts(data)[-1][1], "serif")
        scene = SCENE.replace("label 0", 'set labelfont "default"; label 0')
        self.assertEqual(self.svg_fonts(self.render("-svg", scene, ("-font", "sans")))[0][1],
                         "Times-Roman")

    def test_annotations_inherit_font(self):
        scene = '''plot set labelfont "monospace";
          distance 0 0 0 3 4 0;
          legend "B-factor (Å²)" 0 -5 0 10 1 0 100 blue-white-red;
          key "Kinds" 0 -10 0 10 1 { red "A"; blue "B"; };
          end_plot'''
        fonts = self.svg_fonts(self.render("-svg", scene))
        self.assertGreater(len(fonts), 5)
        self.assertEqual({f for _, f in fonts}, {"monospace"})

    def test_postscript_fonts_and_latin1(self):
        data = self.render("-ps", MIXED)
        self.assertIn("/Helvetica findfont", data)
        self.assertIn("/Courier findfont", data)
        self.assertIn("%%+ font Helvetica", data)
        self.assertIn(r"\305\262", data)
        native = self.render("-ps", args=("-font", "Times-Bold"))
        self.assertIn("/Times-Bold findfont", native)
        if shutil.which("gs"):
            for data in (data, native):
                target = self.path / "font.ps"
                target.write_text(data)
                subprocess.run(["gs", "-q", "-dBATCH", "-dNOPAUSE", "-sDEVICE=nullpage", str(target)],
                               check=True, timeout=30, capture_output=True)

    def test_metapost_fonts_and_axes(self):
        data = self.render("-mp", MIXED + "\n" + SCENE, ("-font", "serif", "-mpaxes"))
        for name in ("cmr10", "cmss10", "cmtt10"):
            self.assertIn(f'defaultfont:="{name}"', data)
        native = self.render("-mp", args=("-font", "cmss10"))
        self.assertIn('defaultfont:="cmss10"', native)
        if shutil.which("mpost"):
            target = self.path / "font.mp"
            target.write_text(data)
            result = subprocess.run(["mpost", "-interaction=nonstopmode", str(target)], cwd=self.path,
                                    text=True, capture_output=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout[-3000:])

    def test_postscript_fonts_redefined_after_plot_restore(self):
        data = self.render("-ps", MIXED + "\n" + MIXED)
        self.assertEqual(data.count("/MSFont0 /Helvetica findfont"), 2)
        self.assertEqual(data.count("/MSFont1 /Courier findfont"), 2)
        self.assertEqual(data.count("%%+ font Helvetica"), 1)
        if shutil.which("gs"):
            target = self.path / "plots.ps"
            target.write_text(data)
            result = subprocess.run(["gs", "-dBATCH", "-dNOPAUSE", "-sDEVICE=nullpage", str(target)],
                                    capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertNotIn("Substituting font", result.stdout + result.stderr)

    def test_x3d_family_binding_and_quoted_native_name(self):
        scene = MIXED.replace('"sans"', '"DejaVu Sans"')
        for mode in ("-x3d", "-webgl"):
            data = self.render(mode, scene)
            if mode == "-webgl":
                data = html.unescape(re.search(r'<textarea id="molscript-scene-data">(.*?)</textarea>', data, re.S)[1])
            root = ET.fromstring(data)
            proto = root.find('.//ProtoDeclare[@name="Label"]')
            self.assertEqual(proto.find('.//field[@name="family"]').get("type"), "MFString")
            self.assertIsNotNone(proto.find('.//FontStyle/IS/connect[@nodeField="family"]'))
            fonts = root.findall('.//ProtoInstance[@name="Label"]/fieldValue[@name="family"]')
            self.assertEqual([f.get("value") for f in fonts], ['"DejaVu Sans"', '"TYPEWRITER"', '"DejaVu Sans"'])
        for mode in ("-x3dv", "-vrml"):
            data = self.render(mode, scene)
            self.assertIn('family IS family', data)
            self.assertIn('"DejaVu Sans"', data)
        if shutil.which("view3dscene"):
            target = self.path / "font.x3d"
            target.write_text(self.render("-x3d", MIXED))
            result = subprocess.run(["view3dscene", "--write", str(target)], capture_output=True, text=True,
                                    env=dict(os.environ, HOME=str(self.path), XDG_CONFIG_HOME=str(self.path)), timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_invalid_or_inapplicable_names(self):
        for name in ("", " ", "a"*128, "foo/bar", "a;end", 'a"b', "a\\b", "A\nB", "Å-font"):
            self.render("-svg", args=("-font", name), status=1)
        self.render("-svg", args=("-font",), status=1)
        self.render("-svg", SCENE.replace("label 0", 'set labelfont "a;end"; label 0'), status=1)
        for mode in ("-ps", "-mp"):
            self.render(mode, args=("-font", "DejaVu Sans"), status=1)
        self.render("-pov", args=("-font", "sans"), status=1)
        self.render("-pov", MIXED, status=1)
        self.render("-raster3d", args=("-font", "Helvetica"), status=1)
        if "-png" in HELP:
            self.render("-png", args=("-font", "sans"), status=1)
        self.assertEqual(self.render("-check", MIXED), "")

    def test_raster3d_two_stroke_fonts(self):
        default = self.render("-raster3d")
        roman = self.render("-raster3d", args=("-font", "roman"))
        mono = self.render("-raster3d", args=("-font", "monospace"))
        self.assertEqual(default, roman)
        self.assertNotEqual(roman, mono)
        # A ring alone is not sufficient: all ordinary letters must have strokes.
        self.assertGreater(roman.count("\n3\n"), 60)
        self.assertGreater(self.render("-raster3d", SCENE.replace("Wi 2.47 Å²", "ABC")).count("\n3\n"), 10)

    @unittest.skipUnless("FreeType support: enabled" in HELP, "requires FreeType")
    def test_image_font_file_choice(self):
        fonts = [Path('/usr/share/fonts/truetype/dejavu') / f for f in ('DejaVuSans.ttf', 'DejaVuSerif.ttf')]
        if not all(p.exists() for p in fonts):
            self.skipTest("DejaVu Sans and Serif fixtures not installed")
        images = []
        env = dict(os.environ, MOLSCRIPT_IMAGE_BACKEND="egl")
        env.pop("DISPLAY", None)
        for font in fonts:
            target = self.path / "font.png"
            result = subprocess.run([MOL, "-s", "-png", "-labelfont", "freetype", "-fontfile", str(font),
                                     "-out", str(target)], input=SCENE, text=True, capture_output=True,
                                    env=env, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)
            images.append(target.read_bytes())
        self.assertNotEqual(*images)


if __name__ == "__main__":
    unittest.main(verbosity=2)
