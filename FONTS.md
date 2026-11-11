# Output fonts (2.3)

Select a font on the command line, or change it for subsequent annotations in
the script. Existing scripts retain their previous default fonts.

```bash
./molscript -svg -font sans -in examples/ras_gdp.in -out ras_gdp_sans.svg
```

```text
set labelfont "sans", labelsize 18;
label 0 0 0 "Distance / Å";
push;
set labelfont "monospace";
label 0 -3 0 "CA 42";
pop;
```

`-font NAME` supplies the initial font for each plot. A script's `set labelfont`
overrides it; `push`/`pop` save and restore the selection. Ordinary labels,
measurement labels, legends and categorical keys all inherit it. Depth-sorted
outputs retain the font assigned when each label was drawn. Explicit
`set labelfont "default"` restores the original backend default, not the
command-line override.

## Portable families

These lowercase presets map to fonts appropriate for each output format:

| Selection | SVG | PostScript | MetaPost | VRML / X3D / WebGL |
| --- | --- | --- | --- | --- |
| `default` | Times-Roman | Times-Roman | cmr10 | SERIF |
| `serif` | serif | Times-Roman | cmr10 | SERIF |
| `sans` or `sans-serif` | sans-serif | Helvetica | cmss10 | SANS |
| `monospace` | monospace | Courier | cmtt10 | TYPEWRITER |

These select families, not identical font files across machines or formats.
Use native names or an explicit FreeType font file when a particular face is
required. Font selection does not change the backend's existing label sizing,
alignment or rotation rules. In particular, MetaPost still uses its font's
native size rather than MolScript's `labelsize`; axis labels use the font
selected at the end of the plot.

## Native names

- **SVG:** `-font "DejaVu Sans"`, for example. The family is written into the
  SVG, not embedded. The browser or SVG rasterizer must have it available;
  otherwise its normal font substitution applies. PNG converted from SVG uses
  this choice, not the OpenGL font options.
- **PostScript:** `-font Helvetica-Bold`, `Times-Italic`, `Courier`, or another
  PostScript font name. Fonts are declared as required resources, not embedded.
  They must be available to the interpreter. Latin-1 re-encoding retains Å and
  ². A missing font may be substituted by the interpreter.
- **MetaPost:** use a TeX font metric name, such as `cmss10` or `cmtt10`. The
  font metrics and the renderer's corresponding fonts must be installed.
  Å/² composition follows the selected font and does not require TeX typesetting.
- **VRML / X3D / WebGL:** native family names are emitted as a quoted MFString
  and resolved by the viewer. Availability and substitution are viewer-specific;
  the portable families are the safest choices. See the
  [X3D FontStyle specification](https://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/text.html#FontStyle).

Names may contain 1-127 ASCII letters, digits, spaces, hyphens, underscores or
dots; leading/trailing spaces are rejected. PostScript and MetaPost names must
not contain spaces. These restrictions prevent names from becoming executable
output syntax. MolScript checks syntax, not font installation. Legacy Greek
`labelmask` runs continue to use Symbol in SVG/PostScript.

## OpenGL and raster images

PNG, JPEG, GIF, SGI and raster EPS use the OpenGL label renderer. Keep the
existing `-labelfont bitmap` default, or select a real font file with FreeType:

```bash
make -C code -j4 USE_FREETYPE=1
./molscript -png -labelfont freetype \
  -fontfile /usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf \
  -in examples/ras_gdp.in -out ras_gdp_serif.png
```

The same options work with interactive `-gl`. Choose the actual bold or italic
font file if that face is wanted. File selection is global for the rendering
run, not per label. Bitmap mode has one bundled face. Both modes support Å/²;
FreeType can draw additional Unicode characters present in the chosen font.
`-font` and non-default `set labelfont` selections are rejected in these modes
instead of pretending to select a system font by family name.

## Raster3D and POV-Ray

Raster3D accepts `default` or `roman` for proportional stroke letters and
`monospace` for fixed-pitch stroke letters. These are emitted as geometry, so
the Raster3D renderer needs no fonts installed. Both work in the basic build
without OpenGL or a display. The font tables are bundled from freeglut 3.4.0
with their original permissive licence notices. This also fixes the old
dependence on incompatible private GLUT/freeglut font structures.

POV-Ray currently exports labels as comments, so it has no rendered-text font
selection. A non-default font selection is rejected rather than ignored.
`-check` validates name syntax but cannot check another backend's font support
or a viewer's installed fonts.

## Examples and tests

The [gallery](examples/index.html) has serif, sans-serif and monospace previews
made from [font_labels.in](examples/font_labels.in). Regenerate them and run
the font tests with:

```bash
python3 scripts/generate-font-examples.py
python3 scripts/test-fonts.py
python3 scripts/build-html-docs.py
```

Generation needs CairoSVG. Tests use installed Ghostscript, MetaPost and
view3dscene when available; the FreeType image comparison needs DejaVu Sans
and Serif plus an EGL-capable complete build.
