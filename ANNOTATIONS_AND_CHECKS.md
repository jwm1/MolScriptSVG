# Legends, checking and SVG groups (2.3)

See [FIGURE_TOOLS.md](FIGURE_TOOLS.md) for vertical/custom-tick legends,
categorical keys, measurements, adaptive sampling and reusable views.

## Legends

The native command is:

```text
legend "Title (units)" X Y Z WIDTH HEIGHT LOW HIGH ramp;
```

`X Y Z` is the lower-left corner in the transformed molecular coordinate
frame, in Angstroms. The bar runs along positive X; its height runs along
positive Y. This is ordinary scene geometry, not a screen overlay: it affects
automatic framing, can be clipped or occluded, and rotates with a 3D scene.
Leave space for the title above and tick labels below. `position` followed by
an atom selection can replace the three position numbers, as for other vectors.

`ramp` accepts `blue-white-red`, `alphafold`, `alphafold-gradient`, `rainbow`,
or `from colour to colour`. The discrete `alphafold` key requires LOW=0 and
HIGH=100; its four swatches have widths proportional to their ranges and ticks
at 0, 50, 70, 90 and 100. The intervals are <=50, >50 to 70, >70 to 90, >90.
Other ramps use 256 colour samples and five equally spaced numeric ticks.
The legend and molecular colours must be given the same range and ramp.

For example, inside a plot:

```text
set colourparts on,
    residuecolour amino-acids b-factor 0 100 alphafold;
coil amino-acids;
push;
set labelsize 12;
legend "pLDDT (B column)" -16 -30 0 32 1.5 0 100 alphafold;
pop;
```

Legends use the current label size and line colour. They use opaque swatches,
centred unrotated text without label offsets, no depth cueing, and solid
0.5-point ticks in vector output. These local settings do not change later
drawing commands. Font appearance and lighting in 3D/image outputs follow
the selected backend; a legend is not a colour-calibration target under 3D
lighting. Labels may require external fonts in POV-Ray, as ordinary labels do.

MolAuto can generate the command with the exact range it chose:

```bash
./molauto -nice -alphafold \
  -legend -16 -30 0 32 1.5 model.pdb > model.in
./molscript -svg -in model.in -out model.svg
```

`-legend X Y Z WIDTH HEIGHT` requires `-bfactor`, `-alphafold` or
`-alphafold_gradient`. MolAuto writes editable commands with a 12-point
label size. Position is explicit, not automatically collision-avoiding.
For a fixed `window`, reserve space for both the molecule and legend.

Examples: [B-factor](examples/ras_bfactor_legend.svg),
[four confidence bins](examples/ras_confidence_legend.svg),
[continuous confidence](examples/ras_confidence_gradient_legend.svg).
Confidence examples use synthetic values, not an AlphaFold prediction.

## Checking without rendering

```bash
./molscript -check -in model.in
```

Coordinate and object paths in scripts are relative to the working directory,
not to the script. For the supplied examples run from `examples` instead:

```bash
cd examples
../molscript -check -in ras_std.in
```

The checker executes parsing, selections, transforms and the vector geometry
pipeline. It writes diagnostics only to stderr, creates no output file, and
does not open X11 or initialize OpenGL. Do not combine `-check` with an output
format or `-out`. It is not merely a syntax check, and large scenes still need
the memory and computation used to construct their geometry.

It reports selected atom/residue counts for drawing commands, empty drawing
selections, empty colour/radius/transform selections, missing CA/P backbone
atoms, distance breaks, fragments too short for their drawing command, empty
plots, and geometry extending beyond an explicit window/slab. Existing syntax,
invalid-number, coordinate and degenerate-geometry errors remain fatal.
Warnings include lexer location information. An intentional crop still warns.

Exit status is **0** for no warnings, **2** for warnings, **1** for errors.
`-s` suppresses ordinary progress messages but not check results. Fatal errors
stop checking; the checker does not attempt recovery or rewrite the script.

Bounds use the existing geometry extent calculation. They are conservative
for cartoons and do not measure glyph boxes or stroke overhang. The checker
does not test external fonts, WebGL runtimes, 3D lighting, or every backend's
capabilities, and does not infer missing coordinates or alter selections.

## Semantic SVG groups

SVG output now records each graphics command automatically. A metadata entry
in namespace `urn:molscript:svg` has a stable document-local ID such as
`plot-1-command-3`, a command kind, and a snapshot of its resolved selection.
The snapshot includes molecule/model, chain, residue name/type, and selected
atom names/alternate locations for atom commands. Residue commands list the
selected residues, not just the atoms used to build their geometry. Paired
atom selections are recorded separately, in consumption order.

Contiguous depth-sorted fragments are enclosed in groups such as:

```xml
<g id="plot-1-fragment-7"
   data-command="plot-1-command-3" data-kind="helix">
  <title>helix</title>
  <!-- unchanged graphical primitives -->
</g>
```

A command can have several groups because other geometry passes in front of
and behind it. Grouping a whole helix in one place would change the image.
Fragment IDs are unique across plots; all fragments of one command share
`data-command`. Empty or fully clipped selections may have metadata but no
visible fragment. Labels outside the plot clip retain their own groups.

This is useful for inspecting SVGs or selecting related fragments in an SVG
editor/script. It does not make every command one editor layer. Metadata
captures resolved identities rather than the original selection expression;
explicit lines, objects and legends have command kinds but no atom selection.
To omit metadata and semantic groups, use `-svg -svgplain`.

## Tests

```bash
python3 scripts/test-annotations.py
```

Tests cover numeric keys and exact discrete colours, custom ramps, validation,
MolAuto generation, several backends, checker diagnostics/exit statuses,
output-file protection, escaped selection metadata, multiple plots, and
pixel-identical rendering with and without semantic groups. CairoSVG and
Pillow enable the pixel test; Ghostscript and MetaPost enable external checks.
