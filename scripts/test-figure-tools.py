#!/usr/bin/env python3
"""Measurements, adaptive curves, scalar interpolation, keys and reusable views."""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
MOL = os.environ.get("MOLSCRIPT", str(ROOT / "molscript"))
AUTO = os.environ.get("MOLAUTO", str(ROOT / "molauto"))
SVG = "{http://www.w3.org/2000/svg}"


class FigureTools(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name)
        self.pdb = self.path / "chain.pdb"
        self.pdb.write_text("".join(
            f"ATOM  {i:5d}  CA  ALA A{i:4d}    {x:8.3f}{y:8.3f}{0:8.3f}{1:6.2f}{b:6.2f}           C\n"
            for i, (x, y, b) in enumerate([(0, 0, 0), (3, 1, 100), (6, 0, 0), (9, 1, 100)], 1)
        ) + "END\n")

    def run_scene(self, scene, *options, status=0):
        p = subprocess.run([MOL, "-s", *options], input=scene, text=True,
                           capture_output=True, cwd=self.path, timeout=30)
        self.assertEqual(p.returncode, status, p.stderr)
        return p.stdout

    def labels(self, output):
        return ["".join(e.itertext()) for e in ET.fromstring(output).iter(SVG + "text")]

    def test_measurements(self):
        scene = '''plot set labelsize 12;
          distance 0 0 0 3 4 0;
          distance 0 0 0 3 4 0 scale 0.1 "nm";
          angle 3 0 0 0 0 0 0 4 0;
          connector 0 -2 0 4 -2 0 "contact & guide";
          end_plot'''
        output = self.run_scene(scene, "-svg")
        self.assertEqual(self.labels(output), ["5.00 Å", "0.50 nm", "90.00 deg", "contact & guide"])
        self.assertGreater(len(list(ET.fromstring(output).iter(SVG + "line"))), 10)
        for mode in ("-ps", "-mp", "-pov", "-x3d", "-vrml", "-r"):
            data = self.run_scene(scene, mode)
            self.assertTrue(data)
            if mode == "-x3d": ET.fromstring(data)

    def test_selection_measurement_and_numeric_following_position(self):
        scene = f'''plot read m "{self.pdb}";
          distance position res-atom "A1" CA position res-atom "A2" CA;
          distance position res-atom "A1" CA 0 4 0;
          distance 0 0 0 position b-factor 100 100;
          legend "key" position res-atom "A1" CA 10 1 0 100 alphafold;
          end_plot'''
        labels = self.labels(self.run_scene(scene, "-svg"))
        self.assertIn("3.16 Å", labels)
        self.assertIn("4.00 Å", labels)
        self.assertIn("6.08 Å", labels)

    def test_unit_labels(self):
        scene = '''plot label 0 0 0 "Å Å² Å"; end_plot'''
        self.assertEqual(self.labels(self.run_scene(scene, "-svg")), ["Å Å² Å"])
        ps = self.run_scene(scene, "-ps")
        self.assertIn("ISOLatin1Encoding", ps)
        self.assertIn(r"\305 \305\262 \305", ps)
        mp = self.run_scene(scene, "-mp")
        self.assertEqual(mp.count("draw fullcircle scaled 2.2"), 3)
        self.assertIn('scaled 0.65 shifted (0,4)', mp)
        self.assertIn("Å Å² Å", self.run_scene(scene, "-x3d"))
        auto = subprocess.check_output([AUTO, "-bfactor", "-legend", "0", "0", "0", "10", "1", str(self.pdb)], text=True)
        self.assertIn("B-factor (Å²)", auto)
        for name, data, command in (
            ("units.ps", ps, ["gs", "-q", "-dBATCH", "-dNOPAUSE", "-sDEVICE=nullpage"]),
            ("units.mp", mp, ["mpost", "-interaction=nonstopmode"]),
        ):
            if shutil.which(command[0]):
                target = self.path / name
                target.write_text(data)
                result = subprocess.run([*command, str(target)], cwd=self.path,
                                        capture_output=True, text=True, timeout=30)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_measurement_errors(self):
        for command in ("angle 0 0 0 0 0 0 1 0 0;",
                        'distance 0 0 0 1 0 0 scale -1 "nm";'):
            self.run_scene("plot " + command + " end_plot", "-svg", status=1)
        labels = self.labels(self.run_scene("plot angle -1 0 0 0 0 0 1 0 0; end_plot", "-svg"))
        self.assertIn("180.00 deg", labels)

    def test_vertical_custom_ticks_and_keys(self):
        scene = '''plot set labelsize 12;
          legend "score" -5 -10 0 1 20 0 100 alphafold-gradient vertical
            ticks { 0 "low"; 70 "confident"; 100 "high"; };
          key "structure" 3 -10 0 2 18 vertical { red "helix"; blue "strand"; grey 0.5 "coil"; };
          key "chains" -5 -15 0 12 1 { red "A"; blue "B"; };
          end_plot'''
        output = self.run_scene(scene, "-svg")
        for word in ("low", "confident", "high", "helix", "strand", "coil", "A", "B"):
            self.assertIn(word, self.labels(output))
        self.assertEqual(len(list(ET.fromstring(output).iter(SVG + "polygon"))), 261)
        self.run_scene(scene, "-x3d")

    def test_tick_and_key_validation(self):
        for tail in ('ticks { 20 "a"; 10 "b"; }', 'ticks { 101 "outside"; }'):
            self.run_scene(f'plot legend "key" 0 0 0 20 2 0 100 alphafold {tail}; end_plot', "-svg", status=1)
        self.run_scene('plot key "empty" 0 0 0 20 2 {}; end_plot', "-svg", status=1)
        out = self.run_scene('plot legend "key" 0 0 0 20 2 0 100 alphafold ticks {}; end_plot', "-svg")
        self.assertEqual(self.labels(out), ["key"])

    def curve(self, state="", ramp="blue-white-red", command="turn", scalar=True):
        colour = f"b-factor 0 100 {ramp}" if scalar else "red"
        return f'''plot area -10 -10 10 10; window 20;
          read m "{self.pdb}";
          set colourparts on, residuecolour amino-acids {colour}, coilradius 0,
              segments 4, segmentsize 100, depthcue 0, {state or 'curvetolerance 0'};
          {command} amino-acids; end_plot'''

    def test_adaptive_sampling(self):
        counts = []
        for tol in ("0.2", "0.01", "0.001"):
            out = self.run_scene(self.curve("curvetolerance " + tol), "-svg")
            counts.append(len(list(ET.fromstring(out).iter(SVG + "path"))))
        self.assertLess(counts[0], counts[1])
        self.assertLess(counts[1], counts[2])
        for value in ("-1", "1e-100"):
            self.run_scene(self.curve("curvetolerance " + value), "-svg", status=1)

    def paths(self, scene):
        root = ET.fromstring(self.run_scene(scene, "-svg"))
        return [list(map(float, re.findall(r"-?\d+(?:\.\d+)?", p.get("d"))))
                for p in root.iter(SVG + "path") if " C " in p.get("d", "")]

    def test_beziers_are_exact_hermite_and_hit_calphas(self):
        # With these equally spaced x coordinates and splinefactor=1, x=3*(i+t).
        ys, dy = [0, 1, 0, 1], [1, 0, 0, 1]
        for setting in ("segments 2", "segments 24", "curvetolerance 0.01"):
            paths = self.paths(self.curve(setting))
            for x, y in ((0, 0), (3, 1), (6, 0), (9, 1)):
                endpoints = [pair for p in paths for pair in (p[:2], p[-2:])]
                self.assertIn([x, y], endpoints)
            for p in paths:
                for u in (0, 0.25, 0.5, 0.75, 1):
                    weights = [(1-u)**3, 3*u*(1-u)**2, 3*u*u*(1-u), u**3]
                    x, y = [sum(weights[j]*p[2*j+k] for j in range(4)) for k in (0, 1)]
                    i = min(2, int(x/3))
                    t = x/3-i
                    expected = ((2*t**3-3*t*t+1)*ys[i] + (-2*t**3+3*t*t)*ys[i+1]
                                + (t**3-2*t*t+t)*dy[i] + (t**3-t*t)*dy[i+1])
                    self.assertAlmostEqual(y, expected, delta=0.0002)

    def test_coil_smoothing_is_explicit(self):
        turn = self.paths(self.curve(command="turn"))
        unsmoothed = self.paths(self.curve("smoothsteps 0", command="coil"))
        smoothed = self.paths(self.curve(command="coil"))
        self.assertEqual(turn, unsmoothed)
        self.assertNotEqual(turn, smoothed)
        self.run_scene(self.curve("smoothsteps -1"), "-svg", status=1)

    def test_beziers_do_not_cross_chain_gaps(self):
        text = self.pdb.read_text().splitlines(True)
        for i in (2, 3):
            text[i] = text[i][:30] + f"{float(text[i][30:38])+5:8.3f}" + text[i][38:]
        self.pdb.write_text("".join(text))
        scene = self.curve().replace("window 20", "window 40")
        paths = self.paths(scene)
        self.assertTrue(paths)
        for p in paths:
            # Window change halves the projected coordinates.
            self.assertTrue(p[-2] <= 1.5 or p[0] >= 5.5, p)

    def test_zero_tolerance_is_legacy(self):
        first = self.run_scene(self.curve(), "-svg")
        second = self.run_scene(self.curve("curvetolerance 0, colourinterpolate off"), "-svg")
        self.assertEqual(first, second)

    def test_scalar_ramp_interpolation_and_categorical_boundaries(self):
        for command in ("turn", "helix", "strand"):
            smooth = self.run_scene(self.curve("colourinterpolate on", command=command), "-svg")
            colours = set(re.findall(r"rgb\([^)]+\)", smooth))
            self.assertGreater(len(colours), 8)
            self.assertIn("rgb(255,255,255)", smooth)
        for scalar, ramp in ((True, "alphafold"), (False, "blue-white-red")):
            off = self.run_scene(self.curve("colourinterpolate off", ramp=ramp, scalar=scalar), "-svg")
            on = self.run_scene(self.curve("colourinterpolate on", ramp=ramp, scalar=scalar), "-svg")
            self.assertEqual(set(re.findall(r"rgb\([^)]+\)", off)), set(re.findall(r"rgb\([^)]+\)", on)))

    def test_scalar_metadata_reassignment(self):
        scene = self.curve("colourinterpolate on").replace("turn amino-acids;", "set residuecolour amino-acids blue; turn amino-acids;")
        out = self.run_scene(scene, "-svg")
        self.assertNotIn("rgb(255,255,255)", out.replace('fill="rgb(255,255,255)"', ""))

    def test_view_roundtrip_and_protection(self):
        view = self.path / "my view.inc"
        transform = 'transform atom * by centre position atom * by rotation z 32; transform atom * by translation 1 2 3;'
        body = f'read m "{self.pdb}"; {transform} cpk atom CA;'
        original = self.run_scene("plot " + body + " end_plot", "-svg", "-viewout", str(view))
        text = view.read_text()
        self.assertIn("macro view_header", text)
        self.assertIn("macro view_transform", text)
        repeat = f'@"{view}"\nplot $view_header read m "{self.pdb}"; transform atom * $view_transform; cpk atom CA; end_plot'
        second = self.run_scene(repeat, "-svg")
        def circles(data):
            return [e.attrib for e in ET.fromstring(data).iter(SVG + "circle")]
        self.assertEqual(circles(original), circles(second))
        self.run_scene("plot " + body + " end_plot", "-svg", "-viewout", str(view), status=1)
        self.assertEqual(view.read_text(), text)
        self.run_scene("plot " + body + " end_plot", "-check", "-viewout", str(view), status=1)

    def test_ambiguous_view_rejected(self):
        scene = f'plot read m "{self.pdb}"; transform res-atom "A1" CA by translation 1 0 0; cpk atom CA; end_plot'
        view = self.path / "bad.inc"
        self.run_scene(scene, "-svg", "-viewout", str(view), status=1)
        self.assertFalse(view.exists())
        self.run_scene('plot set labelsize 12; end_plot', "-svg", "-viewout", str(view), status=1)
        self.assertFalse(view.exists())

    def test_view_cannot_be_overwritten_by_image(self):
        path = self.path / "same.svg"
        self.run_scene(self.curve(), "-svg", "-viewout", str(path), "-out", str(path), status=1)
        self.assertIn("macro view_header", path.read_text())

    def test_molauto_options_and_view(self):
        view = self.path / "my view.inc"
        self.run_scene(self.curve(), "-svg", "-viewout", str(view))
        p = subprocess.run([AUTO, "-bfactor", "-colourinterpolate", "-curvetolerance", ".02",
                            "-view", str(view), str(ROOT / "examples/ras.pdb")], text=True, capture_output=True)
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertIn("$view_transform", p.stdout)
        self.assertNotIn("by centre", p.stdout)
        self.run_scene(p.stdout, "-svg")


if __name__ == "__main__":
    unittest.main(verbosity=2)
