# Headless labels and geometry checks

These features are included in MolScriptSVG 2.2. They were developed and
tested in `molscriptSVG_3`, then carried into the `MolScriptSVG_2.2` release
tree. The original `MolscriptSVG` directory is unchanged.

## Font choices

The complete build defaults to `USE_FREETYPE=0` and `-labelfont bitmap`.
It includes a small printable-ASCII bitmap font and needs no font library.
Both EGL image export and interactive OpenGL use the same renderer, without
calling GLUT's display-dependent font functions.

Build with `USE_FREETYPE=1` to enable `-labelfont freetype`. This requires
FreeType development headers and `pkg-config` metadata (`freetype2`).
Supply an explicit readable font file with `-fontfile`; no system font is
silently chosen. The build tracks changes to this option, so changing it
rebuilds the affected objects without requiring a clean build.

From this directory, build and render the comparison images:

```bash
make -C code -j4 USE_FREETYPE=1
cd examples
env -u DISPLAY MOLSCRIPT_IMAGE_BACKEND=egl ../molscript \
  -png -labelfont bitmap -in ras_gdp.in -out ras_gdp_bitmap.png
env -u DISPLAY MOLSCRIPT_IMAGE_BACKEND=egl ../molscript \
  -png -labelfont freetype \
  -fontfile /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf \
  -in ras_gdp.in -out ras_gdp_freetype.png
```

Previews: [bitmap](examples/ras_gdp_bitmap.png),
[FreeType](examples/ras_gdp_freetype.png).

Both renderers currently produce one-bit glyph masks through `glBitmap`.
FreeType improves glyph shapes and metrics but does not add grayscale text
antialiasing. Existing label-size selection maps to 10, 12 or 18 pixels.
The bundled font supports ASCII 32-126 and substitutes `?` for other bytes.
FreeType receives individual byte values as character codes; UTF-8 decoding,
kerning and complex-script shaping are not implemented. SVG/PS text is
unaffected by these options.

## Geometry and image contexts

Straight helix, strand, coil and cylinder backbones use deterministic
perpendicular normals when curvature cannot supply one. Coincident adjacent
coordinates, non-finite coordinates and undefined/reversing tangents are
rejected with diagnostics instead of zero-vector assertions or NaNs.
These checks do not repair malformed molecular coordinates.

View-axis coils now render in SVG, PS and MetaPost. VRML/X3D cylinders and
sticks use a valid rotation axis when antiparallel to their reference axis.
Planar scenes receive a nonzero default slab depth.

`MOLSCRIPT_IMAGE_BACKEND` accepts `auto` (default), `egl` or `glut`. Auto tries
EGL first and falls back to GLUT; explicit modes never silently switch.
GLUT requires a working X display. When an EGL context lacks accumulation
buffers, `-accum` averages jittered RGB samples in software. Label positions
remain unjittered, matching the existing accumulation path.

## Regression tests

Run from this directory, after building. Pillow is required for image checks;
the live browser test requires Chrome/Chromium and access to the X_ITE CDN.

```bash
python3 scripts/test-svg.py
python3 scripts/test-backends-molauto.py
python3 scripts/test-rendering.py
python3 scripts/test-output-formats.py --images
MOLSCRIPT_TEST_WEBGL_LIVE=1 python3 scripts/test-webgl-browser.py
```

- SVG: 13 tests, including rendered transparency and clipping checks.
- Backends/MolAuto: 13 tests for parsing, format output and option handling.
- Rendering: 15 tests with subcases for straight and nearly straight geometry
  on three axes, malformed coordinates, bitmap/FreeType labels in PNG/JPEG/GIF/
  SGI, label spacing, font errors, accumulation, backend selection, GLUT
  fallback and interactive startup/resize/Escape.
- Browser: three tests using real DOMParser and the generated loader. The
  live test checks nonempty X_ITE scene roots, nonuniform rendered pixels and
  absence of captured console errors. Without the live flag, that test skips.
- Format smoke test: 20 example inputs in nine scene formats, XML parsing,
  available external renderers and odd-sized headless image decoding.

`MOLSCRIPT` selects an alternate executable; `MOLAUTO` does the same for
MolAuto. `MOLSCRIPT_TEST_FONT` selects the test font and `CHROME` selects the
browser. X11 tests require a reachable `DISPLAY` and `xdpyinfo`; resize/Escape
also requires `xdotool`. Missing optional dependencies produce explicit skips.
These tests create and close their own windows, not existing user windows.

On 23 October 2026, the four unittest suites initially reported 43 passes and
one skip with FreeType and live browser checks enabled. After installing
`xdotool`, all 15 rendering tests passed, bringing the combined result to 44
passes with no skips. The resize/Escape test verifies the new window dimensions
and sends key-down separately from key-up: MolScript closes the window on
key-down, so a combined key command could fail while releasing the key.
All 180
example scene outputs passed the format smoke test, with image and external
reader checks. The SVG, backend/MolAuto and applicable headless/geometry tests
also passed an AddressSanitizer/UndefinedBehaviorSanitizer full GL/FreeType
build; leak detection was disabled for legacy process-lifetime allocations,
and its three X11 tests skipped because that environment lacked display access.
A separate minimal build passed all 13 backend/MolAuto tests and the four
geometry tests; image and interactive tests correctly skipped without OpenGL.

Interactive mouse handling and browser navigation are still untested.
The geometry cases are targeted regressions, not exhaustive fuzz coverage.
See [SVG_REVIEW.md](SVG_REVIEW.md) for other outstanding limitations.

## 2.2 release verification

The `MolScriptSVG_2.2` full build with FreeType repeated all 44 unittest cases
successfully, including real X11/xdotool and live Chrome/X_ITE checks. All 180
scene-format checks and the four headless image decodes also passed.
The example regeneration script refreshed PS, SVG, MetaPost, POV-Ray, XML X3D,
Classic X3D, WebGL and the SVG/POV-Ray PNG previews for all 20 inputs.
The release build used the checked-in parser sources; Bison was not installed
on the local host. The new CI workflow installs Bison but has not yet run on
GitHub. Earlier sanitizer results above apply to the same rendering code,
before the release metadata and documentation changes.
