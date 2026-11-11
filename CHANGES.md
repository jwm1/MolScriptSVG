MolscriptSVG Changes
====================

MolScriptSVG 2.3 (unreleased)
----------------------------

* Add -font and set labelfont for SVG, PostScript, MetaPost, VRML and X3D/WebGL,
  with portable families and native font names. Preserve per-label state,
  push/pop, unit symbols and existing defaults; document FreeType image fonts.
* Offer proportional and monospace Raster3D stroke fonts in every build.
  Bundle permissively licensed freeglut font tables instead of interpreting
  incompatible private GLUT structures, fixing missing Raster3D letters.
* Add font-family gallery previews, external-reader and font-state tests.
* Use proper Å and Å² symbols in distance labels and B-factor legend titles.
  Add UTF-8 decoding for bitmap/FreeType and stroke labels, Latin-1 PostScript
  fonts, and TeX-independent MetaPost ring/superscript composition. Avoid signed
  byte indexing in Raster3D labels.
* Preserve exact Hermite controls in SVG/PS/MetaPost thin backbone Beziers,
  including colour boundaries and adaptively sampled intervals. Use an
  unsmoothed turn with C-alpha markers in the thin-backbone example.
* Accept smoothsteps 0 to disable coordinate smoothing explicitly; retain the
  default of two. Document interpolation versus smoothing and unit-font limits.
* Provide HTML companions for the Markdown documentation, with local HTML-only
  documentation navigation, a regeneration script and link checks.
* Refresh external manual links, use local example files instead of remote
  GitHub raw URLs, and update GIF/libgd documentation to reflect expiry of the
  historical GIF/LZW patents. Preserve original library notices as history.

* Add distance/angle annotations, explicit unit scaling and labelled connectors.
* Add optional per-interval adaptive centreline sampling with curvetolerance.
* Add scalar-ramp interpolation along cartoons without blending categorical
  palettes; expose sampling/interpolation controls through MolAuto.
* Add vertical legends, explicit ticks/captions and categorical keys.
* Add -viewout export as editable header/transform macros and MolAuto -view reuse;
  reject ambiguous mixed coordinate frames and preserve existing view files.
* Support quoted include paths and fix res-atom lexer-stack cleanup and numeric
  vectors following position selections. Add figure-tool tests and examples.

* Add portable scalar legends with explicit position, dimensions, range and
  ramp, plus MolAuto -legend generation using the selected scalar bounds.
* Add -check diagnostics without image output or a display; report empty
  selections, missing backbone atoms, short fragments and clipping.
* Add semantic SVG command metadata and depth-preserving fragment groups;
  -svgplain omits them. Add regression tests and three legend examples.

* Use native coloured cylinders in X3D/WebGL, VRML and POV-Ray, with only
  terminal caps; use radial cylinder normals in other 3D outputs.
* Add circular, smoothly shaded coil tubes and POV-Ray cubic sphere sweeps.
* Emit depth-subdivided Bezier paths for thin coils/turns in SVG, PS and MetaPost.

* Add MolAuto B-factor, exact AlphaFold-style and continuous confidence colours.
* Add reusable native named ramps and colour ligand atoms by their own values.
* Support residue-coloured cylinder meshes with terminal caps only.
* Increase VRML/X3D colour precision to preserve palette values.
* Fix yacc action terminators for regeneration with modern Bison.
* Recognize modern PDB DNA residue names such as DA and DT.
* Add scalar-colour tests, synthetic-confidence examples and documentation.

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
