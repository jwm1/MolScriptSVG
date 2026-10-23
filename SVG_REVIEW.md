# MolScriptSVG review

Review date: 23 October 2026. Scope: SVG, other output backends, shared parsing,
and MolAuto. Changes were developed in `molscriptSVG_3` and are included in
the `MolScriptSVG_2.2` release tree. The original `MolscriptSVG` directory
was not edited.
The source baseline was commit `17f57d89f2289f5552556cc91aac5825a1fdc1ca`
plus the existing uncommitted SVG transparency changes, which were retained.

## Fixes

| Area | Problem | Change |
| --- | --- | --- |
| SVG, PS, MetaPost | Coloured point objects advanced by one triplet instead of a position/colour pair, causing invalid colours and out-of-bounds reads. | Advance by two triplets. |
| SVG and PS labels | Short masks were read beyond their allocation. PS also copied an incorrectly sized mask into an unused temporary buffer. | Bound mask lookups and remove the PS temporary copy. |
| SVG transparency | Fill and stroke opacity were applied separately. Overlap made a half-transparent red point about 75% opaque at its border. Highlights had the same issue. | Apply opacity once to a group containing the primitive and its decorations. |
| SVG surfaces | Same-colour seam-sealing strokes darkened translucent facet edges. | Keep sealing strokes for opaque facets only. |
| SVG transparency | Very small transparency values were discarded; fully invisible primitives remained in output. | Preserve small values and omit invisible geometry/labels. |
| SVG clipping | `labelclip off` labels were still inside the plot clip group. | Emit those labels after closing the clip group. |
| SVG and PS bounds | A negative x origin doubled as an uninitialized bounding-box sentinel. | Track initialization separately; retain negative/multiple plot bounds. |
| SVG, PS, MetaPost areas | Zero-size areas reached division by zero. | Reject non-finite coordinates and nonpositive dimensions before projection setup. |
| MetaPost matrix | The printed row-vector matrix contradicted the documented column-vector equation. | Transpose the printed matrix and identify its input as already-transformed view coordinates. |
| VRML and X3D | Coloured-point objects closed one node too many, causing an indentation assertion. | Balance node closures and count points rather than triplets. |
| X3D labels | Escaped quotes terminated tokens prematurely and broke XML conversion. | Preserve escaped characters while scanning strings. |
| PNG | Final writes could longjmp into a returned initialization function. | Install a fresh error handler around row/end writes and free resources on failure. |
| OpenGL images | The GLUT fallback skipped `glutInit`; pixel pack alignment was implicit. | Initialize GLUT on fallback and set pack alignment to one byte. |
| Shared regex | Legacy V8 regex functions exported POSIX names with incompatible signatures. | Prefix symbols to prevent shared-library interposition. |
| SGI | Headers used native byte order; RLE tables assumed 32-bit `long`. Files were unreadable on this host. | Serialize fixed-width big-endian integers and initialize header padding. |
| Lexer | Reading EOF again after consuming the final token dereferenced a freed input source. | Make repeated EOF reads safe, including input without a final newline. |
| MolAuto options | NaN, infinity, overflow, zero/negative windows, surplus filenames and trailing unknown/repeated options were accepted. | Validate values and require all arguments to be consumed. |
| MolAuto structure | Short final helices/strands escaped cleanup; the first residue could be counted twice. | Count each residue once and process the final segment. |
| MolAuto titles | Empty COMPND/MOLECULE text triggered a string assertion. | Omit empty titles; also guard against an empty residue list. |
| Headless labels | GLUT fonts required an X11 display even on EGL. | Bundle bitmap glyphs and add optional FreeType rasterization without GLUT font calls. |
| Degenerate geometry | Straight backbones and axis-aligned primitives produced zero normals; malformed coordinates caused assertions. | Choose deterministic perpendiculars where defined; reject coincident, non-finite or undefined geometry with diagnostics. |
| Planar OpenGL scenes | Zero scene depth produced an invalid slab. | Supply a minimum default slab depth. |
| EGL accumulation | Contexts without accumulation buffers failed with `-accum`. | Accumulate RGB samples in software when necessary. |
| WebGL loading | Nested XML parser errors went unnoticed; the Interchange profile did not include Text. | Detect parsererror descendants and declare Immersive in both X3D encodings. |

## Verification

- Full build with OpenGL/EGL, PNG, JPEG and GIF support succeeded.
- `scripts/test-svg.py`: 13 tests passed, including an actual CairoSVG/Pillow pixel check.
- `scripts/test-backends-molauto.py`: 13 tests passed, with additional subcases for presets, invalid inputs and output modes.
- Both suites and the applicable headless/geometry tests passed with AddressSanitizer and UndefinedBehaviorSanitizer on a full OpenGL/FreeType build. Leak detection was disabled for legacy process-lifetime allocations; X11 tests were skipped in the sanitizer environment.
- `scripts/test-rendering.py`: all 15 passed on the host EGL/GLX paths after installing `xdotool`. Tests cover bitmap/FreeType labels in four image formats, font errors, geometry, accumulation, forced GLUT fallback, interactive startup and verified resize/Escape exit. The keyboard test releases Escape separately because key-down closes the target window.
- `scripts/test-webgl-browser.py`: all three passed in Chrome, including the opt-in live X_ITE render check and malformed XML error handling.
- All 20 example inputs generated successfully in nine formats: PS, MetaPost, SVG, VRML, XML X3D, Classic X3D, WebGL HTML, POV-Ray and Raster3D. SVG/XML and embedded WebGL XML were parsed.
- Ghostscript and MetaPost processed `ras_std`; view3dscene accepted its XML X3D; POV-Ray rendered its scene.
- With `DISPLAY` unset, PNG, JPEG, GIF and SGI exports of `ras_std` decoded through Pillow at the requested odd dimensions, 201 x 199.
- A PNG write to `/dev/full` exited with a write-error diagnostic rather than jumping into an expired stack frame.
- All 20 SVG/PNG example pairs and X3D/XML, Classic X3D and WebGL assets were regenerated in this directory. Bitmap and FreeType labelled PNG comparisons were added. Other checked formats were generated in temporary test directories.

The regeneration helper stages each SVG/PNG pair and checks both before
replacing old previews. CairoSVG is preferred, with Inkscape as a fallback.
It no longer deletes a valid preview before attempting rasterization.

## Remaining issues

1. OpenGL labels use monochrome glyphs and legacy size selection. Bundled
   glyphs cover printable ASCII; FreeType strings remain byte-oriented, not
   full UTF-8 or shaped text. See RENDERING_TESTS.md for details.
2. SVG transparency is per primitive with painter-style depth sorting. It does
   not implement order-independent transparency for intersecting surfaces.
   Adjacent translucent triangles can still show antialiasing seams, visible
   in `examples/svg_transparency_svg.png`. The fix removes dark overlapping
   sealing strokes, not every rasterizer's shared-edge artifact.
3. The X3D converter remains a purpose-built, line-oriented converter for
   internally generated scene data. The quoted-label and coloured-point fixes
   are not evidence that it handles arbitrary Classic X3D syntax.
4. Interactive startup, GLX fallback, accumulation and browser loading/rendering
   now have exercised tests, including resize/Escape automation with `xdotool`.
   Mouse interaction and browser navigation remain untested.
5. This review is not an exhaustive memory-safety audit. Straight and nearly
   straight synthetic backbones now render; coincident or reversing paths
   with undefined tangents fail cleanly. Arbitrary malformed geometry still
   needs broader fuzz testing. No new Python-map or mmCIF features were added.

## Reproduce

From this directory:

```bash
make -C code -j4
python3 scripts/test-svg.py
python3 scripts/test-backends-molauto.py
python3 scripts/test-rendering.py
python3 scripts/test-webgl-browser.py
python3 scripts/test-output-formats.py --images
python3 scripts/regenerate-svg-examples.py
```

The two regression suites accept `MOLSCRIPT` and, for MolAuto tests, `MOLAUTO`
environment variables to select a separate instrumented build. The format
check requires Pillow with `--images`, reports missing external renderers as
skipped, and does not overwrite example assets.
See [RENDERING_TESTS.md](RENDERING_TESTS.md) for optional FreeType, X11 and
network-dependent browser checks.
