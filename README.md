MolScriptSVG 2.1.2
==================

    Copyright (C) 1997-1998 Per J. Kraulis
    Copyright (C) 2026 James W. Murray

<table>
  <tr>
    <td>
      <img src="docs/images/ras_std.jpg" title="ras_std">
    </td>
    <td>
      <img src="docs/images/ras_cyl.jpg" title="ras_cyl">
    </td>
    <td>
      <img src="docs/images/ras_gdp_balls.jpg" title="ras_gdp_balls">
    </td>
  </tr>
</table>

MolScriptSVG is a MolScript-derived program for displaying molecular 3D
structures, such as proteins, in both schematic and detailed
representations.

This fork also includes SVG, X3D and WebGL-oriented output backends. SVG is
written directly from the MolScript geometry pipeline and does not require any
external SVG library. X3D output is available as both XML (`-x3d`) and
Classic encoding (`-x3dv`). Both Classic and XML X3D are now generated
natively inside MolScriptSVG. The `-webgl` mode writes an HTML
viewer that embeds the native XML X3D scene and displays it in the browser
using the X_ITE WebGL runtime.

Fork-specific modifications in this repository are copyright
James W. Murray. Those modifications were developed with assistance
from OpenAI Codex.

The documentation is at [http://pekrau.github.io/MolScript/](http://pekrau.github.io/MolScript/).

Background
----------

MolScript has for a long time been a standard tool in the science of
macromolecular structures. [The paper describing
it](http://dx.doi.org/10.1107/S0021889891004399 "MOLSCRIPT: a program
to produce both detailed and schematic plots of protein structures.")
appears as number 82 in the list of the Nature feature article ["The
Top 100 Papers. Nature explores the most-cited research of all
time"](http://www.nature.com/news/the-top-100-papers-1.16224) by
Richard Van Noorden, Brendan Maher & Regina Nuzzo published [30 Oct
2014](http://www.nature.com/nature/journal/v514/n7524/index.html).

I have written a blog post [MolScript: A story of success and
failure](http://kraulis.wordpress.com/2014/11/03/molscript-a-story-of-success-and-failure/),
describing the history behind its rise and fall.

Open Source
-----------

MolScriptSVG is distributed as a fork under the MIT license in this GitHub
repository. The original MolScript project was later released as Open
Source. The Nature Top-100-list helped push that process into action.

Version 2.1.2
-------------

The first version of MolScript (written in Fortran 77) was released in
1991, and its current version (2.1.2, written in C) in 1998.

This fork keeps the upstream MolScript 2.1.2 language and executable name,
but it is no longer a pristine 1998 code snapshot. The original OpenGL/image
build expected a 1990s GLUT/GLX setup and no longer built cleanly on a modern
Linux system. The `code/Makefile.complete` file has now been modernized to use
`pkg-config` for OpenGL, PNG, JPEG and GIF dependencies.

I have tested the `Makefile.basic` file, which builds an executable
with support for PostScript, SVG, Raster3D, VRML and X3D. It works, at least on
Ubuntu 12.04. I have also verified that the [Raster3D software
(v3.0)](http://skuld.bmsc.washington.edu/raster3d/html/raster3d.html)
still works with MolScript.

The SVG, X3D and WebGL backends are available in both the basic and complete
builds:

```bash
./molscript -svg -in examples/ras_std.in -out ras_std.svg
./molscript -x3d -in examples/ras_std.in -out ras_std.x3d
./molscript -x3dv -in examples/ras_std.in -out ras_std.x3dv
./molscript -webgl -in examples/ras_std.in -out ras_std.webgl.html
```

`-x3dv`, `-x3d` and `-webgl` are all generated natively by MolScriptSVG.
The `-webgl` output is a self-contained HTML wrapper around the native XML X3D
scene, but it still loads the X_ITE JavaScript runtime from a CDN at view time.

For a modern OpenGL-enabled build, use:

```bash
cd code
make -f Makefile.complete
```

To enable OpenGL-backed image outputs, build with the relevant feature flags:

```bash
make -f Makefile.complete USE_IMAGE=1 USE_PNG=1
make -f Makefile.complete USE_IMAGE=1 USE_PNG=1 USE_JPEG=1 USE_GIF=1
```

The image-output path now prefers a headless EGL OpenGL context, which avoids
opening an X11 window and can render PNG, JPEG and GIF output without a working
X11 display. If EGL initialization fails, MolScriptSVG falls back to the hidden
GLUT window path for compatibility with older setups.

Fork Changes
------------

This fork differs from the historical 2.1.2 release in a few important ways:

* Added direct SVG output with `-svg`.
* Added X3D output in both XML (`-x3d`) and Classic (`-x3dv`) encodings.
* Added HTML WebGL viewer output with `-webgl`, backed by the X3D scene exporter.
* Modernized the OpenGL build in `code/Makefile.complete` for current Linux
  systems using `pkg-config`.
* Restored OpenGL-backed PNG, JPEG and GIF image export on modern systems.
* Updated the examples gallery to work from a local filesystem checkout and to
  include the generated SVG assets.
* XML X3D and WebGL are now generated natively, without an external converter.

Release Status
--------------

The current tree has been smoke-tested for:

* `-svg`
* `-x3d`
* `-x3dv`
* `-webgl`
* OpenGL interactive mode via `-gl`
* OpenGL-backed `-png`, `-jpeg` and `-gif`

The generated example outputs under `examples/` are intended to be part of the
repository for comparison and regression checking.

For a quick smoke test after building:

```bash
./scripts/smoke-test.sh
```

Future plans
------------

I have very little time to work on MolScript currently. Other projects
are more pressing. If anyone is willing to "take over" (i.e. fork) and
continue developing MolScript, I would be very pleased.

Here are some possible items for a roadmap for future development of MolScript:

* Continue modernizing and testing the OpenGL/image implementation on current systems.

* Prepare a proper Debian package for easier installation.

* Add a proper interactive interface to the OpenGL implementation. The
  script language was nice once upon a time, but today it must be
  considered as user-hostile and cumbersome.

* Set up a web service producing images from input scripts using MolScript.


Reference
---------

    Per J. Kraulis
    MOLSCRIPT: a program to produce both detailed and schematic plots of
    protein structures.
    J. Appl. Cryst. (1991) 24, 946-950

This paper is now available under Open Access: [PDF](docs/kraulis_1991_molscript_j_appl_cryst.pdf)

[DOI:10.1107/S0021889891004399](http://dx.doi.org/10.1107/S0021889891004399)

[Entry at J. Appl. Cryst. web site](http://scripts.iucr.org/cgi-bin/paper?S0021889891004399)
