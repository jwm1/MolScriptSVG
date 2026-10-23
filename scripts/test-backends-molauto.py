#!/usr/bin/env python3
"""Regression tests for MolAuto and non-SVG output modes."""
import os
import re
from pathlib import Path
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
MOLSCRIPT = str(Path(os.environ.get("MOLSCRIPT", ROOT / "molscript")).resolve())
MOLAUTO = str(Path(os.environ.get("MOLAUTO", ROOT / "molauto")).resolve())
PDB = str(ROOT / "examples/ras.pdb")


class RegressionTests(unittest.TestCase):
    def auto(self, *args):
        return subprocess.run([MOLAUTO, *args], capture_output=True, text=True, timeout=30)

    def render(self, mode, scene, success=True):
        with tempfile.TemporaryDirectory() as tmp:
            source, target = Path(tmp) / "test.in", Path(tmp) / "output"
            source.write_text(scene)
            result = subprocess.run([MOLSCRIPT, mode, "-in", str(source), "-out", str(target)],
                                    capture_output=True, text=True, timeout=30)
            if success:
                self.assertEqual(result.returncode, 0, result.stderr)
                return target.read_text()
            self.assertGreater(result.returncode, 0, result.stderr)
            return result.stderr

    def test_invalid_numbers(self):
        for option, values in (("-window", ["0", "-1", "nan", "inf", "1e999"]),
                               ("-translate", ["nan", "inf", "1e999"])):
            for value in values:
                with self.subTest(option=option, value=value):
                    args = [option, value] + (["0", "0"] if option == "-translate" else [])
                    result = self.auto(*args, PDB)
                    self.assertGreater(result.returncode, 0, result.stderr)
                    self.assertEqual(result.stdout, "")

    def test_extra_arguments(self):
        for tail in (["-typo"], [PDB], ["-window", "20", "-window", "30"]):
            with self.subTest(tail=tail):
                result = self.auto(PDB, *tail)
                self.assertGreater(result.returncode, 0, result.stderr)
                self.assertEqual(result.stdout, "")

    def test_empty_pdb(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "empty.pdb"
            path.write_text("END\n")
            result = self.auto(str(path))
            self.assertGreater(result.returncode, 0, result.stderr)
            self.assertIn("MolAuto error", result.stderr)

    def test_terminal_short_helix_and_empty_title(self):
        for count in (2, 3):
            with self.subTest(residues=count), tempfile.TemporaryDirectory() as tmp:
                header = list(" " * 80)
                for start, text in ((0, "HELIX "), (15, "ALA"), (19, "A"),
                                    (21, "   1"), (27, "ALA"), (31, "A"), (33, f"{count:4d}")):
                    header[start:start + len(text)] = text
                atoms = [f"ATOM  {i:5d}  CA  ALA A{i:4d}    {i*3.6:8.3f}{i%2:8.3f}{0:8.3f}  1.00 20.00           C\n"
                         for i in range(1, count + 1)]
                path = Path(tmp) / "short.pdb"
                path.write_text("COMPND    MOLECULE: ;\n" + "".join(header) + "\n" + "".join(atoms) + "END\n")
                result = self.auto(str(path))
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertEqual("  helix " in result.stdout, count == 3, result.stdout)
                ET.fromstring(self.render("-svg", result.stdout))

    def test_presets_generate_renderable_scripts(self):
        for preset in ("-nice", "-publication", "-colourblind", "-ss_palette", "-ss_ca", "-ss_hb"):
            with self.subTest(preset=preset):
                result = self.auto(preset, "-window", "60", "-translate", "1", "2", "3",
                                   "-rotate", "1", "0", "0", "0", "1", "0", "0", "0", "1", PDB)
                self.assertEqual(result.returncode, 0, result.stderr)
                ET.fromstring(self.render("-svg", result.stdout))

    def test_coloured_points(self):
        scene = "plot window 10; object inline;\nPC 2\n-2 0 0 1 0 0\n2 0 0 0 0 1\nQ\nend_plot\n"
        for mode in ("-ps", "-mp", "-vrml", "-x3d", "-povray", "-raster3d"):
            with self.subTest(mode=mode):
                self.assertTrue(self.render(mode, scene))

    def test_x3d_coloured_points_are_well_formed(self):
        root = ET.fromstring(self.render("-x3d",
            "plot window 10; object inline;\nPC 2\n-2 0 0 1 0 0\n2 0 0 0 0 1\nQ\nend_plot\n"))
        self.assertIsNotNone(root.find(".//PointSet/Color"))

    def test_x3d_quoted_label(self):
        scene = 'plot window 10; slab 10; label 0 0 0 "say \\"hi, [there]\\""; end_plot\n'
        root = ET.fromstring(self.render("-x3d", scene))
        field = root.find('.//ProtoInstance[@name="Label"]/fieldValue[@name="c"]')
        self.assertIsNotNone(field)
        self.assertEqual(field.get("value"), '"say \\"hi, [there]\\""')

    def test_no_final_newline(self):
        for mode in ("-ps", "-svg", "-mp", "-vrml", "-x3d", "-pov"):
            with self.subTest(mode=mode):
                self.render(mode, "plot window 10; slab 10; label 0 0 0 test; end_plot")

    def test_ps_negative_multiplot_bounds(self):
        out = self.render("-ps", "plot area -200 0 -100 100; window 10; label 0 0 0 left; end_plot\n"
                          "plot area 0 0 100 100; window 10; label 0 0 0 right; end_plot\n")
        self.assertIn("%%BoundingBox: -200 0 100 100", out)

    def test_metapost_matrix_maps_origin_to_image_centre(self):
        out = self.render("-mp", "plot area 0 0 200 300; window 10; label 0 0 0 test; end_plot\n")
        rows = re.findall(r"%   \[([^\]]+)\]", out)
        matrix = [[float(v) for v in row.split()] for row in rows[:4]]
        self.assertEqual([row[3] for row in matrix], [100, 150, 0, 1])

    def test_ps_short_and_long_masks(self):
        for mask, label in (("g", "abcdefghijk"), ("g" * 128, "a")):
            self.render("-ps", f'plot window 10; set labelmask "{mask}"; label 0 0 0 "{label}"; end_plot')

    def test_zero_area(self):
        for mode in ("-ps", "-mp"):
            with self.subTest(mode=mode):
                self.assertIn("invalid area", self.render(mode,
                    "plot area 0 0 0 100; window 10; label 0 0 0 test; end_plot", False))


if __name__ == "__main__":
    unittest.main(verbosity=2)
