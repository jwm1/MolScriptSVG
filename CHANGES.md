MolscriptSVG Changes
====================

Unreleased
----------

* Renamed this fork to MolScriptSVG 2.1.2 while keeping the executable name
  `molscript`.
* Added native SVG output with `-svg`.
* Added Classic X3D output with `-x3dv`.
* Added XML X3D output with `-x3d`, generated natively inside MolScriptSVG.
* Added WebGL HTML output with `-webgl`, embedding native XML X3D for use
  with the X_ITE browser runtime.
* Modernized OpenGL build support in `code/Makefile.complete` for current Linux
  systems.
* Restored OpenGL-backed PNG, JPEG and GIF output on modern systems.
* Added EGL-first headless OpenGL image rendering, with fallback to hidden GLUT
  window rendering.
* Updated example outputs and local example gallery support.
