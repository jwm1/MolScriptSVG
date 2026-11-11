#!/usr/bin/env python3
"""Legends, non-rendering checks and depth-preserving SVG metadata."""
import io
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
MOL = os.environ.get("MOLSCRIPT", str(ROOT / "molscript"))
AUTO = os.environ.get("MOLAUTO", str(ROOT / "molauto"))
SVG = "{http://www.w3.org/2000/svg}"
MS = "{urn:molscript:svg}"
PDB = ROOT / "examples/ras.pdb"


class Annotations(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name)

    def run_scene(self, scene, *options, status=0):
        p = subprocess.run([MOL, "-s", *options], input=scene, text=True,
                           capture_output=True, timeout=30,
                           env=dict(os.environ, DISPLAY=""))
        self.assertEqual(p.returncode, status, p.stderr)
        return p

    def test_discrete_legend(self):
        scene = 'plot set segmentsize 100; legend "pLDDT & confidence" -15 -5 0 30 2 0 100 alphafold; end_plot'
        root = ET.fromstring(self.run_scene(scene, "-svg").stdout)
        labels = ["".join(e.itertext()) for e in root.iter(SVG + "text")]
        for label in ("0", "50", "70", "90", "100", "pLDDT & confidence"):
            self.assertIn(label, labels)
        polygons = list(root.iter(SVG + "polygon"))
        self.assertEqual(len(polygons), 4)
        text = ET.tostring(root).decode()
        for rgb in ("238,132,83", "249,220,77", "117,199,244", "33,82,206"):
            self.assertIn("rgb(" + rgb + ")", text)
        self.assertEqual(len(list(root.iter(SVG + "line"))), 5)

    def test_continuous_and_custom_legends(self):
        for ramp in ("blue-white-red", "alphafold-gradient", "from rgb 0 0 1 to rgb 1 0 0"):
            scene = f'plot set segmentsize 100; legend "B-factor" 0 0 0 30 2 10 90 {ramp}; end_plot'
            root = ET.fromstring(self.run_scene(scene, "-svg").stdout)
            self.assertEqual(len(list(root.iter(SVG + "polygon"))), 256)
            self.assertIn("50", [e.text for e in root.iter(SVG + "text")])

    def test_invalid_legends(self):
        for values in ("0 2 0 100 alphafold", "20 2 10 10 blue-white-red",
                       "20 2 10 100 alphafold", "20 2 0 1e999 blue-white-red"):
            self.run_scene(f'plot legend "key" 0 0 0 {values}; end_plot', "-svg", status=1)

    def test_legend_state_restoration(self):
        legend = 'legend "key" 0 0 0 20 2 0 100 alphafold;'
        scene = f'plot set transparency 0.4, linewidth 3, depthcue 0; {legend} line 0 10 0 to 2 10 0; end_plot'
        root = ET.fromstring(self.run_scene(scene, "-svg").stdout)
        self.assertEqual(len(list(root.iter(SVG + "text"))), 6)
        group = next(g for g in root.iter(SVG + "g") if g.get("data-kind") == "line")
        self.assertTrue(any(float(g.get("opacity", "1")) == 0.6 for g in group.iter(SVG + "g")))
        self.assertIn('stroke-width:3', ET.tostring(group).decode())

    def test_portable_legend(self):
        scene = 'plot legend "confidence" -15 0 0 30 2 0 100 alphafold; end_plot'
        for mode in ("-ps", "-mp", "-pov", "-x3d", "-vrml", "-r"):
            p = self.run_scene(scene, mode)
            self.assertTrue(p.stdout)
            if mode == "-x3d":
                ET.fromstring(p.stdout)
            if mode == "-ps" and shutil.which("gs"):
                subprocess.run(["gs", "-q", "-dBATCH", "-dNOPAUSE", "-sDEVICE=nullpage", "-"],
                               input=p.stdout, text=True, check=True, capture_output=True)
            if mode == "-mp" and shutil.which("mpost"):
                source = self.path / "legend.mp"
                source.write_text(p.stdout)
                run = subprocess.run(["mpost", "-interaction=nonstopmode", str(source)],
                                     cwd=self.path, capture_output=True, text=True, timeout=30)
                self.assertEqual(run.returncode, 0, run.stdout[-2000:])

    def test_check_clean_and_no_display(self):
        scene = f'plot read mol "{PDB}"; transform atom * by centre position atom *; coil amino-acids; end_plot'
        p = self.run_scene(scene, "-check")
        self.assertEqual(p.stdout, "")
        self.assertIn("0 warning(s)", p.stderr)
        self.assertIn("selected residues", p.stderr)

    def test_check_empty_short_and_clipped(self):
        base = f'read mol "{PDB}";'
        for scene, warning in (
            (f'plot {base} cpk atom DOESNOTEXIST; end_plot', "empty atom selection"),
            (f'plot {base} helix residue 1; end_plot', "too short to draw"),
            (f'plot window 1; slab 1; {base} cpk atom CA; end_plot', "outside the explicit window"),
        ):
            p = self.run_scene(scene, "-check", status=2)
            self.assertEqual(p.stdout, "")
            self.assertIn(warning, p.stderr)

    def test_check_missing_backbone_and_break(self):
        lines = PDB.read_text().splitlines(True)
        lines = [s for s in lines if not (s.startswith("ATOM") and s[12:16].strip() == "CA"
                                         and s[22:26].strip() == "2")]
        # Translate residue 5's CA far away, leaving all other atoms unchanged.
        lines = [s[:30] + f'{999:8.3f}' + s[38:] if s.startswith("ATOM") and
                 s[12:16].strip() == "CA" and s[22:26].strip() == "5" else s for s in lines]
        pdb = self.path / "broken.pdb"
        pdb.write_text("".join(lines))
        p = self.run_scene(f'plot read m "{pdb}"; coil amino-acids; end_plot', "-check", status=2)
        self.assertIn("missing CA", p.stderr)
        self.assertIn("backbone distance breaks", p.stderr)

    def test_check_errors_and_output_protection(self):
        target = self.path / "keep.svg"
        target.write_text("do not replace")
        self.run_scene('plot line 0 0 0 to 1 0 0; end_plot', "-check", "-out", str(target), status=1)
        self.assertEqual(target.read_text(), "do not replace")
        self.run_scene("plot nonsense; end_plot", "-check", status=1)
        self.run_scene("plot line 0 0 0 to 1 0 0; end_plot", "-check", "-png", status=1)

    def test_check_empty_state_and_transform(self):
        scene = f'''plot read m "{PDB}";
          set atomcolour atom ABSENT red, atomradius atom ABSENT 1,
              residuecolour type ABSENT blue;
          transform atom ABSENT by translation 1 0 0;
          line 0 0 0 to 1 0 0; end_plot'''
        p = self.run_scene(scene, "-check", status=2)
        for command in ("atomcolour", "atomradius", "residuecolour", "transform"):
            self.assertIn("selection for " + command, p.stderr)

    def test_molauto_legend(self):
        for mode in ("-bfactor", "-alphafold", "-alphafold_gradient"):
            p = subprocess.run([AUTO, mode, "-legend", "-15", "-25", "0", "30", "2", str(PDB)],
                               text=True, capture_output=True, timeout=30)
            self.assertEqual(p.returncode, 0, p.stderr)
            self.assertIn('legend "', p.stdout)
            ET.fromstring(self.run_scene(p.stdout, "-svg").stdout)
        p = subprocess.run([AUTO, "-legend", "0", "0", "0", "20", "2", str(PDB)],
                           text=True, capture_output=True)
        self.assertNotEqual(p.returncode, 0)

    def test_groups_selection_and_multiple_plots(self):
        plot = f'plot read "m&x" "{PDB}"; helix from 15 to 25; ball-and-stick in type GDP; label 0 0 0 "note"; end_plot'
        root = ET.fromstring(self.run_scene(plot + "\n" + plot, "-svg").stdout)
        commands = list(root.iter(MS + "command"))
        self.assertEqual(len(commands), 6)
        self.assertIn("m&x/model=", commands[0].text)
        self.assertIn("GDP", commands[1].text)
        self.assertIn("[", commands[1].text)
        ids = [e.get("id") for e in root.iter() if e.get("id")]
        self.assertEqual(len(ids), len(set(ids)))
        command_ids = {e.get("id") for e in commands}
        groups = [e for e in root.iter(SVG + "g") if e.get("data-command")]
        self.assertTrue(groups)
        self.assertTrue(all(g.get("data-command") in command_ids for g in groups))

    def test_groups_preserve_primitives_and_pixels(self):
        try:
            import cairosvg
            from PIL import Image
        except ImportError:
            self.skipTest("CairoSVG and Pillow required")
        scene = f'plot read m "{PDB}"; transform atom * by centre position atom *; set transparency 0.3; helix from 15 to 25; coil amino-acids; ball-and-stick in type GDP; label 0 0 0 "test"; end_plot'
        grouped = self.run_scene(scene, "-svg").stdout
        plain = self.run_scene(scene, "-svg", "-svgplain").stdout
        tags = {SVG + s for s in ("path", "polygon", "line", "circle", "text")}
        def primitives(data):
            return [(e.tag, e.attrib, e.text) for e in ET.fromstring(data).iter() if e.tag in tags]
        self.assertEqual(primitives(grouped), primitives(plain))
        images = [Image.open(io.BytesIO(cairosvg.svg2png(bytestring=s.encode()))).convert("RGBA")
                  for s in (grouped, plain)]
        self.assertEqual(images[0].tobytes(), images[1].tobytes())


if __name__ == "__main__":
    unittest.main(verbosity=2)
