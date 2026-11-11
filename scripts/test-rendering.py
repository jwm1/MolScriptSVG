#!/usr/bin/env python3
"""Geometry, headless fonts, accumulation and optional X11 regression tests.

MOLSCRIPT selects the executable. MOLSCRIPT_TEST_FONT selects a FreeType font.
X11 tests skip without a working DISPLAY; keyboard tests also need xdotool.
"""
import io
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import time
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
BINARY = str(Path(os.environ.get("MOLSCRIPT", ROOT / "molscript")).resolve())
FONT = os.environ.get("MOLSCRIPT_TEST_FONT", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")
HELP = subprocess.run([BINARY, "-h"], capture_output=True, text=True).stderr


class SceneTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="molscript-render-test-")
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name)

    def render(self, scene, mode="-svg", args=(), success=True, backend="egl"):
        source, output = self.path / "scene.in", self.path / "output"
        source.write_text(scene)
        env = dict(os.environ, MOLSCRIPT_IMAGE_BACKEND=backend)
        if backend == "egl":
            env.pop("DISPLAY", None)
        result = subprocess.run([BINARY, mode, "-size", "301", "199", *args,
                                 "-in", str(source), "-out", str(output)],
                                cwd=ROOT / "examples", env=env, capture_output=True, text=True, timeout=30)
        if success:
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertNotIn("OpenGL error", result.stderr)
            data = output.read_bytes()
            self.assertTrue(data)
            return data, result.stderr
        self.assertGreater(result.returncode, 0, result.stderr)
        self.assertNotIn("Assertion", result.stderr)
        return result.stderr

    def backbone(self, points, command):
        pdb = self.path / "chain.pdb"
        pdb.write_text("".join(
            f"ATOM  {i:5d}  CA  ALA A{i:4d}    {x:8.3f}{y:8.3f}{z:8.3f}  1.00 20.00           C\n"
            for i, (x, y, z) in enumerate(points, 1)) + "END\n")
        return (f'plot read mol "{pdb}"; transform atom * by centre position atom *; '
                f"{command} from A1 to A{len(points)}; end_plot\n")


class GeometryTests(SceneTests):
    def test_straight_backbones(self):
        for command in ("helix", "strand", "coil", "cylinder"):
            for axis in range(3):
                for count in (3, 5):
                    points = [tuple(i * 3.5 if a == axis else 0 for a in range(3)) for i in range(count)]
                    with self.subTest(command=command, axis=axis, count=count):
                        scene = self.backbone(points, command)
                        for mode in ("-svg", "-ps", "-mp", "-vrml", "-x3d", "-pov", "-raster3d"):
                            data, _ = self.render(scene, mode)
                            if mode in ("-svg", "-x3d"):
                                ET.fromstring(data)

    def test_coincident_and_reversing_backbones_fail_cleanly(self):
        for points in ([(0, 0, 0)] * 3, [(0, 0, 0), (1, 0, 0), (0, 0, 0)]):
            for command in ("helix", "strand", "coil", "cylinder"):
                with self.subTest(command=command, points=points):
                    error = self.render(self.backbone(points, command), success=False)
                    self.assertIn("degenerate geometry", error)

    def test_nearly_straight_backbones(self):
        for command in ("helix", "strand", "coil", "cylinder"):
            with self.subTest(command=command):
                scene = self.backbone([(0, 0, 0), (3.5, 0.001, 0), (7, 0, 0), (10.5, 0.001, 0)], command)
                ET.fromstring(self.render(scene)[0])

    def test_nonfinite_coordinates(self):
        scene = self.backbone([(0, 0, 0), (float('nan'), 0, 0), (7, 0, 0)], 'helix')
        self.assertIn('non-finite', self.render(scene, success=False))


@unittest.skipUnless("-png" in HELP, "requires the complete image build")
class HeadlessTests(SceneTests):
    def test_straight_geometry_in_opengl(self):
        for command in ('helix', 'strand', 'coil', 'cylinder'):
            for axis in range(3):
                with self.subTest(command=command, axis=axis):
                    points = [tuple(i * 3.5 if a == axis else 0 for a in range(3)) for i in range(5)]
                    self.image(self.render(self.backbone(points, command), '-png')[0])

    def image(self, data):
        from PIL import Image
        im = Image.open(io.BytesIO(data)).convert("RGB")
        self.assertEqual(im.size, (301, 199))
        return im

    def font_args(self, mode):
        if mode == "freetype":
            if "FreeType support: enabled" not in HELP:
                self.skipTest("build with USE_FREETYPE=1 for FreeType tests")
            if not Path(FONT).is_file():
                self.skipTest("set MOLSCRIPT_TEST_FONT to a readable font file")
            return ("-labelfont", mode, "-fontfile", FONT)
        return ("-labelfont", mode)

    def label(self, text="Ag 123", size=18):
        return (f'plot window 10; background white; set linecolour red, labelsize {size}; '
                f'label -3 0 0 "{text}"; end_plot\n')

    def check_fonts(self, font):
        args = self.font_args(font)
        for size in (10, 14, 24):
            for mode in ("-png", "-jpeg", "-gif", "-sgi"):
                with self.subTest(font=font, size=size, mode=mode):
                    data, log = self.render(self.label(size=size), mode, args)
                    self.assertIn("Image backend: EGL", log)
                    pixels = self.image(data).getdata()
                    self.assertGreater(sum(r > g + 60 and r > b + 60 for r, g, b in pixels), 20)

    def test_bitmap_fonts_all_image_modes(self):
        self.check_fonts("bitmap")

    def test_freetype_fonts_all_image_modes(self):
        self.check_fonts("freetype")

    def test_spaces_advance_raster_position(self):
        from PIL import ImageChops, Image
        for text in ("AA", "A A"):
            data, _ = self.render(self.label(text), "-png")
            im = self.image(data)
            box = ImageChops.difference(im, Image.new("RGB", im.size, "white")).getbbox()
            self.assertIsNotNone(box)
            if text == "AA":
                tight_width = box[2] - box[0]
            else:
                self.assertGreater(box[2] - box[0], tight_width)

    def test_font_errors(self):
        for args in (("-labelfont", "invalid"), ("-labelfont", "freetype"),
                     ("-fontfile", FONT), ("-labelfont", "freetype", "-fontfile", "/no/such/font")):
            with self.subTest(args=args):
                self.render(self.label(), "-png", args, success=False)

    def test_angstrom_ring_and_utf8_decoding(self):
        from PIL import ImageChops, Image
        for font in ("bitmap", "freetype"):
            args = self.font_args(font)
            for size in (10, 14, 24):
                boxes = []
                for text in ("A", "Å", "Å"):
                    im = self.image(self.render(self.label(text, size), "-png", args)[0])
                    boxes.append(ImageChops.difference(im, Image.new("RGB", im.size, "white")).getbbox())
                self.assertLess(boxes[1][1], boxes[0][1], (font, size, boxes))
                self.assertEqual(boxes[1], boxes[2])
                # Decoding must not turn the two-byte Å into two advancing glyphs.
                self.assertAlmostEqual(boxes[0][2]-boxes[0][0], boxes[1][2]-boxes[1][0], delta=1)
            regular = self.image(self.render(self.label("2"), "-png", args)[0])
            raised = self.image(self.render(self.label("²"), "-png", args)[0])
            self.assertNotEqual(regular.tobytes(), raised.tobytes())

    def test_accumulation_preserves_unjittered_labels(self):
        for font in ("bitmap", "freetype"):
            with self.subTest(font=font):
                args = self.font_args(font)
                plain = self.image(self.render(self.label(), "-png", args)[0])
                data, log = self.render(self.label(), "-png", (*args, "-accum", "4"))
                self.assertEqual(plain.tobytes(), self.image(data).tobytes(), log)

    def test_accumulation_molecule(self):
        scene = (ROOT / "examples/ras_gdp.in").read_text()
        plain = self.image(self.render(scene, "-png")[0])
        data, _ = self.render(scene, "-png", ("-accum", "4"))
        im = self.image(data)
        self.assertNotEqual(plain.tobytes(), im.tobytes())
        self.assertTrue(any(lo < hi for lo, hi in im.getextrema()))

    def test_forced_backend_validation(self):
        self.assertIn("MOLSCRIPT_IMAGE_BACKEND", self.render(self.label(), "-png", backend="typo", success=False))
        env = dict(os.environ, MOLSCRIPT_IMAGE_BACKEND="glut")
        env.pop("DISPLAY", None)
        p = subprocess.run([BINARY, "-png"], input=self.label(), env=env, capture_output=True, text=True, timeout=30)
        self.assertEqual(p.returncode, 1, p.stderr)
        self.assertIn("requires an X11 DISPLAY", p.stderr)


@unittest.skipUnless("-gl" in HELP, "requires the OpenGL build")
class X11Tests(HeadlessTests):
    # Only explicitly selected X11 tests should run, not inherited headless tests.
    def setUp(self):
        SceneTests.setUp(self)
        if not os.environ.get("DISPLAY") or not shutil.which("xdpyinfo"):
            self.skipTest("needs DISPLAY and xdpyinfo; run under xvfb-run")
        p = subprocess.run(["xdpyinfo"], capture_output=True, timeout=10)
        if p.returncode:
            self.skipTest("no accessible X11 display; run under xvfb-run")

    def test_glut_fallback_and_accumulation(self):
        for font in ("bitmap", "freetype"):
            for args in ((), ("-accum", "4")):
                data, log = self.render(self.label(), "-png", (*self.font_args(font), *args), backend="glut")
                self.assertIn("Image backend: GLUT", log)
                self.assertTrue(any(lo < hi for lo, hi in self.image(data).getextrema()))

    def test_interactive_keyboard(self):
        if not shutil.which("xdotool"):
            self.skipTest("keyboard regression test requires xdotool")
        source = self.path / "interactive.in"
        source.write_text(self.label())
        log_path = self.path / "interactive.log"
        with log_path.open("w") as log:
            process = subprocess.Popen([BINARY, "-gl", "-in", str(source)], stdout=log, stderr=log)
            escape_sent = False
            try:
                window = None
                for _ in range(40):
                    self.assertIsNone(process.poll(), log_path.read_text())
                    search = subprocess.run(["xdotool", "search", "--pid", str(process.pid)], capture_output=True, text=True)
                    if search.returncode == 0 and search.stdout.strip():
                        window = search.stdout.splitlines()[0]
                        break
                    time.sleep(0.1)
                self.assertIsNotNone(window, log_path.read_text())
                subprocess.run(["xdotool", "windowsize", "--sync", window, "420", "320"],
                               check=True, timeout=10)
                time.sleep(0.3)
                self.assertIsNone(process.poll(), log_path.read_text())
                geometry = subprocess.run(["xdotool", "getwindowgeometry", "--shell", window],
                                          check=True, capture_output=True, text=True, timeout=10)
                self.assertIn("WIDTH=420", geometry.stdout)
                self.assertIn("HEIGHT=320", geometry.stdout)
                # Escape closes the window on key-down; key-up cannot target it.
                escape_sent = True
                subprocess.run(["xdotool", "keydown", "--window", window, "Escape"],
                               check=True, timeout=10)
                self.assertEqual(process.wait(timeout=10), 0, log_path.read_text())
                self.assertNotIn("OpenGL error", log_path.read_text())
            finally:
                if escape_sent:
                    subprocess.run(["xdotool", "keyup", "Escape"], check=True, timeout=10)
                if process.poll() is None:
                    process.terminate()
                    process.wait(timeout=10)

    def test_interactive_startup(self):
        source = self.path / 'interactive.in'
        source.write_text(self.label())
        log_path = self.path / 'startup.log'
        with log_path.open('w') as log:
            process = subprocess.Popen([BINARY, '-gl', '-accum', '4', '-in', str(source)], stdout=log, stderr=log)
            try:
                time.sleep(2)
                self.assertIsNone(process.poll(), log_path.read_text())
                self.assertIn('display lists rendered', log_path.read_text())
                self.assertNotIn('OpenGL error', log_path.read_text())
            finally:
                if process.poll() is None:
                    process.terminate()
                    process.wait(timeout=10)


if __name__ == "__main__":
    # Avoid collecting HeadlessTests methods a second time through inheritance.
    suite = unittest.TestSuite()
    for cls in (GeometryTests, HeadlessTests, X11Tests):
        suite.addTests(cls(name) for name in cls.__dict__ if name.startswith("test_"))
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    raise SystemExit(not result.wasSuccessful())
