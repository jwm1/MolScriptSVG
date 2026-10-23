MolscriptSVG Changes
====================

MolScriptSVG 2.2
----------------

Release prepared 23 October 2026. See RELEASE_NOTES_2.2.md for migration notes.

* Fix coloured object point indexing and short label-mask bounds checks.
* Composite translucent primitive fills, outlines and highlights together.
* Disable opaque-only seam sealing for translucent facets; preserve small
  transparency values and omit fully invisible primitives.
* Preserve negative and multiple plot area bounds and reject zero-size areas.
* Honour labelclip off by placing those labels outside the plot clip group.
* Add SVG regression tests, a transparency example, and staged SVG/PNG
  regeneration with CairoSVG support.
* Fix PostScript and MetaPost coloured-point indexing, PostScript label-mask
  memory errors, negative PostScript bounds, and degenerate plot areas.
* Correct the documented MetaPost image-frame matrix convention.
* Fix extra node closures in VRML/X3D coloured-point objects and preserve
  escaped quotes in X3D label conversion.
* Restore a live PNG error-handler stack frame for writes, initialize GLUT
  on image fallback, and explicitly set pixel readback alignment.
* Prefix the bundled regex symbols to avoid collisions with POSIX regex
  functions called by shared libraries.
* Write portable SGI headers and RLE tables on little-endian/64-bit hosts.
* Fix a lexer crash when input has no final newline.
* Reject invalid MolAuto numbers, nonpositive windows and surplus arguments;
  omit empty titles and clean up terminal short secondary-structure segments.
* Render headless OpenGL labels with a bundled bitmap font or optional
  FreeType font; add -labelfont and -fontfile options and USE_FREETYPE builds.
* Add deterministic normals for straight backbones, reject undefined geometry
  with diagnostics, and guard non-finite coordinates and planar scene slabs.
* Fix view-axis coils in SVG/PS/MetaPost and antiparallel cylinder/stick
  rotation axes in VRML/X3D.
* Support accumulation antialiasing on EGL contexts without accumulation
  buffers, and add an explicit EGL/GLUT image-backend selector.
* Declare the X3D Immersive profile needed by labels and detect nested browser
  XML parser errors in the WebGL loader.
* Add MolAuto, cross-backend, headless-font, geometry, accumulation, X11 and
  browser regression tests. See SVG_REVIEW.md and RENDERING_TESTS.md for
  verification results and remaining limitations.

MolScriptSVG 2.1.5
------------------

* Renamed this fork to MolScriptSVG while keeping the executable name
  `molscript`.
* Added native SVG output with `-svg`.
* Added MetaPost source output with `-mp`.
* Added POV-Ray scene output with `-pov` / `-povray`.
* Added `scripts/render-pov-examples.sh` and rendered POV example PNGs.
* Added optional MetaPost axes/tick overlay with `-mpaxes`.
* Added semantic comments to MetaPost output for secondary-structure and
  selected atom-style drawing commands.
* Added Classic X3D output with `-x3dv`.
* Added XML X3D output with `-x3d`, generated natively inside MolScriptSVG.
* Added WebGL HTML output with `-webgl`, embedding native XML X3D for use
  with the X_ITE browser runtime.
* Added new `molauto` options for framing/orientation (`-window`, `-rotate`,
  `-translate`) and palette presets (`-ss_palette`, `-colourblind`,
  `-publication`).
* Modernized OpenGL build support in `code/Makefile.complete` for current Linux
  systems.
* Restored OpenGL-backed PNG, JPEG and GIF output on modern systems.
* Added EGL-first headless OpenGL image rendering, with fallback to hidden GLUT
  window rendering.
* Updated example outputs and local example gallery support, including
  MetaPost source files and POV-Ray rendered example images.

2.1.5 Release Notes
-------------------

This release turns the fork into a modernized, usable MolScript distribution
while keeping compatibility with the historical `molscript` executable and
script language. The main additions are SVG, native X3D/XML and WebGL output,
plus restored OpenGL image export on modern Linux systems.
