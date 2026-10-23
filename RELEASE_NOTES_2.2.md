# MolScriptSVG 2.2

This release focuses on rendering correctness, headless image export and
regression coverage. The command names remain `molscript` and `molauto`.
The MIT license and existing copyright notices are retained.

## New and improved

- Headless OpenGL labels work without X11, using a bundled bitmap font or
  optional FreeType. Use `-labelfont bitmap`, or build with `USE_FREETYPE=1`
  and select `-labelfont freetype -fontfile /path/to/font.ttf`.
- `MOLSCRIPT_IMAGE_BACKEND=auto|egl|glut` selects the image context. EGL
  accumulation antialiasing works even without hardware accumulation buffers.
- Straight and nearly straight backbones have stable normals. Undefined or
  coincident geometry and non-finite coordinates fail with diagnostics rather
  than zero-vector assertions. Axis-aligned coils and cylinders are fixed.
- SVG transparency groups fills, outlines and highlights correctly; clipping,
  label masks, coloured points and negative/multiple plot bounds are fixed.
- PostScript, MetaPost, VRML/X3D, PNG and SGI receive correctness fixes. X3D
  declares the profile required by labels; WebGL reports malformed XML reliably.
- MolAuto validates numeric options and surplus arguments, handles empty
  titles, and correctly cleans up short terminal secondary-structure segments.
- Regression suites cover SVG pixels, MolAuto, scene formats, degenerate
  geometry, both font renderers, accumulation, GLUT, interactive resize/Escape
  and browser loading. Generated example files remain in `examples/`.

## Compatibility and limits

The MolScript language and executable names are unchanged. Stricter validation
may reject input previously accepted accidentally. The default OpenGL label
font now uses bundled glyphs, so its appearance differs from the old GLUT font.
FreeType is optional, not a new mandatory dependency. Both OpenGL label paths
currently use monochrome glyphs and legacy size selection, without UTF-8
decoding or text shaping. SVG and PostScript text are unaffected.

SVG transparency still uses depth-sorted primitives; intersecting translucent
surfaces and shared-edge antialiasing can show artifacts. The internal X3D
converter handles generated scene data, not arbitrary Classic X3D input.
WebGL viewing requires the pinned X_ITE runtime from its CDN. Mouse handling
and browser navigation do not yet have automated coverage.

## Build and validation

See [README.md](README.md) for dependencies and compilation,
[RENDERING_TESTS.md](RENDERING_TESTS.md) for test commands and font previews,
and [SVG_REVIEW.md](SVG_REVIEW.md) for the detailed audit.

The 2.2 release build passed all 44 unittest cases, 180 generated scene-format
checks and headless image decoding. Applicable geometry, image and backend
tests also passed AddressSanitizer/UndefinedBehaviorSanitizer builds with leak
detection disabled. These are targeted regression checks, not a complete
memory-safety audit. The new GitHub Actions workflow covers basic and full
builds; its hosted run remains to be checked after pushing.
