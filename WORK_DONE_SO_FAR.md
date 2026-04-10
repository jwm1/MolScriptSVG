MolScriptSVG Work Summary
=========================

Current baseline
----------------

- Project renamed from `MolScript` to `MolScriptSVG`.
- Executable name kept as `molscript`.
- Current local baseline: `MolScriptSVG 2.1.5` tagged as `v2.1.5`.
- MIT licensing retained, with fork changes attributed to James W. Murray.

Major backend work
------------------

- Added direct SVG output with `-svg`.
- Added MetaPost output with `-mp` / `-metapost`.
- Added X3D output:
  - `-x3d` for XML X3D
  - `-x3dv` for Classic X3D
- Added `-webgl` output using X_ITE-backed HTML wrappers.
- Restored and modernized OpenGL-backed image output:
  - `-png`
  - `-jpeg`
  - `-gif`
  - `-sgi`
  - `-eps`
  - `-epsbw`

OpenGL and image-path modernization
-----------------------------------

- Modernized `Makefile.complete` for current Linux systems.
- Switched dependency discovery to `pkg-config`.
- Fixed the old GLX pixmap-based image path.
- Added EGL-first headless image rendering with fallback to GLUT/X11.
- Verified working `-gl`, `-png`, `-jpeg`, and `-gif` on the local machine.

MetaPost work
-------------

- Added native MetaPost scene output.
- Added semantic comments for:
  - helices
  - strands
  - coils/turns
  - traces
  - ball-and-stick
  - bonds
  - cpk
- Added optional projected axes/ticks overlay via `-mpaxes`.
- Added image-frame transform matrix comments to generated `.mp` figures.

X3D and WebGL stabilization
---------------------------

- Implemented Classic X3D first, then stabilized native XML X3D generation.
- Iterated on WebGL/X_ITE integration until native scenes loaded and displayed reliably.
- Fixed XML validity issues affecting WebGL scene loading.

SVG work
--------

- Added native SVG scene generation.
- Improved SVG/PostScript parity.
- Fixed visible seams with higher coordinate precision and seam-hiding strokes.
- Matched default border line colour to PostScript.
- Added PostScript-style sphere highlight support.

MolAuto improvements
--------------------

- Added framing/orientation options:
  - `-window`
  - `-rotate`
  - `-translate`
- Added palette/preset options:
  - `-ss_palette`
  - `-colourblind`
  - `-publication`

Examples and documentation
--------------------------

- Regenerated and checked in example outputs across the supported backends.
- Added SVG-derived PNGs as `*_svg.png`.
- Added MetaPost example outputs for all example `.in` files.
- Added the `trace` example to the gallery.
- Updated `examples/index.html` to work from a local checkout.
- Added `scripts/regenerate-examples.sh`.
- Added `UNDOCUMENTED_FEATURES.md`.
- Updated README and the documentation HTML pages to use MolScriptSVG branding.
- Added `CHANGES.md` and `.gitignore`.

Current direction
-----------------

MolScriptSVG is now a modernized multi-backend MolScript distribution with:

- vector output
- 3D scene export
- WebGL viewing
- restored OpenGL/image output on modern Linux
- improved examples and documentation

Likely next areas
-----------------

- POV-Ray output backend
- more MetaPost polish
- better label support in 3D backends
- CI smoke tests
