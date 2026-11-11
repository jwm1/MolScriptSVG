# Headless labels and geometry checks

## Output font selection (11 November 2026)

`python3 scripts/test-fonts.py` passes all 11 tests on the complete FreeType
build. Coverage includes portable/native names, safe-name validation, saved
label state, push/pop, per-plot defaults, annotation inheritance, PostScript
font resources across plot save/restore, MetaPost axes, X3D family bindings,
both Raster3D stroke fonts and distinct FreeType image fonts. Ghostscript,
MetaPost and view3dscene accepted the test output.

An isolated basic ASan/UBSan build passes all 10 applicable font tests (the
FreeType test skips; leak detection disabled). This includes actual ordinary
Raster3D letter strokes, not just the angstrom ring. Bundled freeglut tables
replace the incompatible private GLUT structures used previously.

The SVG, backend/MolAuto, scalar, curves, annotations and figure-tool suites
all pass. Rendering passes 13 geometry/headless tests; three X11 interaction
tests skip without an accessible display. The format suite validates 297 scene
outputs and four headless image formats. The three browser tests pass outside
the sandbox, including live X_ITE rendering of the font-only example with
`MOLSCRIPT_TEST_WEBGL_FONT=sans`. This establishes visible text, not identical
font metrics across viewers.

All 33 base SVG/PNG pairs and affected vector/3D scene files were refreshed.
The three portable-family previews were visually inspected. Documentation
checks pass for 49 HTML pages. See [output fonts](FONTS.md) for usage and
remaining backend-specific sizing/installation limits.

## Unit symbols, exact curves and documentation (11 November 2026)

The figure-tool suite now has 17 passing tests, including Å/Å² output, external
Ghostscript/MetaPost acceptance, exact Bezier-versus-Hermite comparisons at
multiple sampling densities, C-alpha interpolation, chain gaps and explicit
zero smoothing. All 17 also pass in an isolated basic ASan/UBSan build, with
leak detection disabled. The existing SVG (13), backend/MolAuto (13), scalar
(7), annotations (13) and curves (4) suites pass on the complete FreeType build.

The rendering suite passes its 13 geometry/headless tests, including unit glyph
decoding, ring placement and superscripts with bitmap and FreeType fonts.
Three X11 interaction tests skip because no accessible display is available.
No live WebGL tests were rerun. The format suite validates 288 scene outputs
and decodes PNG/JPEG/GIF/SGI headless images. All 32 SVG/PNG examples were
refreshed; backbone C-alpha markers, measurement units, B-factor units and
PostScript/MetaPost/bitmap unit glyphs were visually inspected.

`python3 scripts/build-html-docs.py --check` validates local HTML navigation
without network access: no Markdown or HTTP/FTP links, no missing local HTML
targets, and valid heading anchors in generated Markdown companions. External
project links were refreshed against project, publisher and institutional
pages. This is not a guarantee that every external site accepts automated
requests: GNU timed out, PubMed presented a browser challenge, and Paul
Bourke's SGI mirror blocked the crawler. The SGI mirror is also cited by the
UK National Archives format record. Historical library licence text remains
unchanged, with current GIF/LZW patent-expiry context added outside it.

## Figure tools (11 November 2026)

`python3 scripts/test-figure-tools.py`: 13 new tests pass on the complete
FreeType build and isolated basic ASan/UBSan build (leak detection disabled).
Coverage includes measurement values/units and degenerate angles, numeric
selection predicates, vertical/custom/categorical keys, tolerance-controlled
sampling, legacy defaults, scalar-ramp interpolation and categorical boundaries,
reusable-view round trips, ambiguous transforms, and output-file protection.
The annotation (13), scalar (7), SVG (13), backend/MolAuto (13) and curve (4)
suites also pass. Annotation and scalar suites were rerun under sanitizers.

The expanded 32-example suite generated 288 valid scene outputs and decoded
four headless image formats. Four new gallery examples include scene files and
SVG PNG previews. The measurement, legend-layout and smooth-scalar previews
were visually inspected. No live browser/X11 interaction tests were rerun;
the earlier Escape automation limitation remains unchanged. View export is
tested for scripted coordinates and vector framing, not interactive cameras.

## Legends, checking and groups (11 November 2026)

`python3 scripts/test-annotations.py`: 13 tests pass on the FreeType-enabled
complete build and an isolated basic ASan/UBSan build (leak detection disabled).
These cover scalar legends and state restoration, checker diagnostics and
exit statuses, file protection, SVG selection metadata, unique multi-plot IDs,
and identical primitives/pixels with and without semantic groups. Ghostscript
and MetaPost accepted the legend fixture. The existing SVG (13), backend/
MolAuto (13), scalar-colour (7) and curve (4) tests also pass.

The format suite generated 252 outputs from 28 examples and decoded four
headless image formats. All 28 SVG/PNG pairs were refreshed. Three new legend
examples include native scene exports and SVG PNG previews; the discrete key
was visually checked for label spacing, colours and seams. Interactive X11 and
live WebGL tests were not rerun for this change; the earlier Escape automation
limitation below remains unresolved. No new external rendering library is
needed for the implementation.

## Curve-rendering update (11 November 2026)

`python3 scripts/test-curves.py` adds four checks: native cylinder cap
connectivity, round-tube normals, POV-Ray spline rendering, and vector Bezier
paths accepted by Ghostscript/MetaPost. All pass, including on the basic
ASan/UBSan build (leak detection disabled). The seven scalar-colour tests
also pass under sanitizers. The live browser suite was rerun with
`MOLSCRIPT_TEST_WEBGL_INPUT=ras_confidence_cylinders.in` and passed all three
checks using X_ITE. This tests the new native cylinder prototype and tube mesh.

The 13 SVG and 13 backend/MolAuto tests pass, as do 14 of 15 rendering tests.
The resize/Escape test currently times out waiting for synthetic Escape;
the same failure occurs against the unchanged 2.2 binary, including an
isolated retry with visible-window filtering and explicit focus. Those
speculative test changes were not retained. Interactive startup and GLUT
fallback pass. This is an unresolved automation limitation, not a clean
full-suite pass.

All 225 generated scene-format checks pass, plus four image decodes. Existing
gallery scenes and SVG/POV-Ray previews were regenerated; a thin-curve example
was added. OpenGL and POV-Ray cylinder/tube previews were visually inspected.
The following sections retain the earlier scalar-colour and 2.2 results.

## 2.3 development verification (11 November 2026)

Seven new scalar-colour tests cover exact palette boundaries and interpolation,
option errors, automatic/constant ranges, non-finite values, mixed protein/DNA/
ligand data, duplicate residue numbers/insertion codes and cylinder axes.
Run them with `python3 scripts/test-scalar-colours.py`.
The basic and FreeType-enabled complete builds passed the new checks. The
basic ASan/UBSan build also passed all seven (legacy lifetime leak detection
disabled). This new sanitizer run does not cover OpenGL.

The existing 13 SVG, 13 backend/MolAuto, 15 rendering and three browser tests
passed, including X11/xdotool and live Chrome/X_ITE. The expanded example set
passed 216 scene-format checks and PNG/JPEG/GIF/SGI decoding. Cylinder SVG,
PostScript and headless OpenGL PNG previews were visually inspected.
Browser tests exercise the existing general scene, not interactive navigation
of the new coloured-cylinder example. Automated cylinder tests validate export
and axis handling, not pixel-perfect silhouette or cap topology assertions.

The remaining sections document the historical 2.2 review.

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
