#!/usr/bin/env python3
"""Run the generated WebGL loader in headless Chrome.

Offline tests exercise the real DOMParser and loader error handler without X_ITE.
Set MOLSCRIPT_TEST_WEBGL_LIVE=1 to test the CDN runtime and WebGL rendering too.
MOLSCRIPT_TEST_WEBGL_FONT selects a font family for the generated scene.
CHROME selects the browser. Browser launch failures are explicit skips.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
BINARY = str(Path(os.environ.get("MOLSCRIPT", ROOT / "molscript")).resolve())
CHROME = os.environ.get("CHROME") or shutil.which("google-chrome") or shutil.which("chromium")


@unittest.skipUnless(CHROME, "Chrome/Chromium not installed; set CHROME")
class BrowserTests(unittest.TestCase):
    def check_page(self, malformed=False, live=False):
        with tempfile.TemporaryDirectory(prefix="molscript-browser-") as tmp:
            tmp = Path(tmp)
            page = tmp / "scene.html"
            proc = subprocess.run([BINARY, "-webgl", "-font", os.environ.get("MOLSCRIPT_TEST_WEBGL_FONT", "default"),
                                   "-in", os.environ.get("MOLSCRIPT_TEST_WEBGL_INPUT", "ras_std.in"), "-out", str(page)],
                                  cwd=ROOT / "examples", capture_output=True, text=True, timeout=30)
            self.assertEqual(proc.returncode, 0, proc.stderr)
            markup = page.read_text()
            if not live:
                markup = re.sub(r'<script defer src="[^"]+"></script>', '', markup)
            if malformed:
                markup = re.sub(r'(<textarea id="molscript-scene-data">).*?(</textarea>)',
                                r'\1&lt;X3D&gt;&lt;broken&gt;&lt;/X3D&gt;\2', markup, flags=re.S)
            markup = markup.replace('<head>', '''<head><script>
              window.testErrors = [];
              const oldError = console.error;
              console.error = (...args) => { testErrors.push(args.join(' ')); oldError(...args); };
              window.addEventListener('error', event => testErrors.push(event.message));
            </script>''')
            if live:
                markup = markup.replace('contentScale="auto"', 'preserveDrawingBuffer="true" contentScale="auto"')
                condition = """
                  const element = document.querySelector('x3d-canvas');
                  const browser = window.X3D?.getBrowser(element);
                  const canvas = element.shadowRoot?.querySelector('canvas');
                  if (!browser?.currentScene?.rootNodes?.length || !canvas) return false;
                  const gl = canvas.getContext('webgl2') || canvas.getContext('webgl');
                  if (!gl || gl.isContextLost()) return false;
                  const pixels = new Uint8Array(canvas.width * canvas.height * 4);
                  const target = gl.READ_FRAMEBUFFER || gl.FRAMEBUFFER;
                  const binding = gl.getParameter(gl.READ_FRAMEBUFFER_BINDING || gl.FRAMEBUFFER_BINDING);
                  gl.bindFramebuffer(target, null);
                  gl.readPixels(0, 0, canvas.width, canvas.height, gl.RGBA, gl.UNSIGNED_BYTE, pixels);
                  gl.bindFramebuffer(target, binding);
                  let varied = 0;
                  for (let i = 4; i < pixels.length; i += 4)
                    if (pixels[i] !== pixels[0] || pixels[i+1] !== pixels[1] || pixels[i+2] !== pixels[2]) varied++;
                  return varied > 200 && testErrors.length === 0;
                """
            elif malformed:
                condition = "return testErrors.some(e => e.includes('WebGL load failed')) && !document.querySelector('x3d-canvas > X3D');"
            else:
                condition = "return !!document.querySelector('x3d-canvas > X3D Scene IndexedFaceSet, x3d-canvas > X3D Scene Shape') && testErrors.length === 0;"
            markup = markup.replace('</body>', '''<script>
              window.addEventListener('DOMContentLoaded', () => {
                const check = () => { CONDITION };
                const timer = setInterval(() => {
                  try { if (check()) { document.body.dataset.testStatus = 'pass'; clearInterval(timer); } }
                  catch (e) { document.body.dataset.testError = String(e); }
                  document.body.dataset.testErrors = testErrors.join(';');
                }, 100);
              });
            </script></body>'''.replace('CONDITION', condition))
            page.write_text(markup)
            command = [CHROME, '--headless', '--disable-dev-shm-usage', '--no-first-run',
                       '--no-default-browser-check', '--use-angle=swiftshader', '--enable-unsafe-swiftshader',
                       '--user-data-dir=' + str(tmp / 'profile'), '--dump-dom',
                       '--virtual-time-budget=' + ('20000' if live else '2000'), page.as_uri()]
            if os.environ.get('MOLSCRIPT_TEST_CHROME_NO_SANDBOX') == '1':
                command.insert(1, '--no-sandbox')
            proc = subprocess.run(command, capture_output=True, text=True, timeout=60,
                                  env=dict(os.environ, HOME=str(tmp)))
            if proc.returncode and not proc.stdout:
                self.skipTest('browser could not start: ' + proc.stderr[-800:])
            self.assertEqual(proc.returncode, 0, proc.stderr[-1500:])
            self.assertTrue(re.search(r'<body[^>]*data-test-status="pass"', proc.stdout),
                             'Browser loader/render test did not pass. ' + proc.stderr[-1500:] +
                             '\n' + str(re.findall(r'<body[^>]*>', proc.stdout)))

    def test_local_file_loader(self):
        self.check_page()

    def test_malformed_xml_reports_error(self):
        self.check_page(malformed=True)

    @unittest.skipUnless(os.environ.get('MOLSCRIPT_TEST_WEBGL_LIVE') == '1',
                         'set MOLSCRIPT_TEST_WEBGL_LIVE=1 for network-dependent X_ITE/WebGL test')
    def test_live_xite_rendering(self):
        self.check_page(live=True)


if __name__ == '__main__':
    unittest.main(verbosity=2)
