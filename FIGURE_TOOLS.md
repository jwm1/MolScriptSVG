# Figure tools (2.3)

These additions keep the MolScript script as the figure specification. All
positions are explicit. Existing scripts keep fixed sampling and residue-wise
colours unless the new controls are enabled. No new runtime library is needed.

## Distances, angles and connectors

```text
set labelsize 12, linecolour black, linewidth 0.8, labeloffset 0 1 0;
distance position res-atom 200 O1B position res-atom 201 MG;
angle position res-atom 200 O1B position res-atom 201 MG position res-atom 17 OG;
connector position res-atom 200 N2 2 5 0 "GDP";
```

`distance vector vector;` draws a dashed connector with its length at the
midpoint, to two decimal places, in angstroms (`Å`). `angle vector vector
vector;` measures the angle at the **second** point, draws dashed arms and an
arc, and labels degrees (`deg`). Coincident angle arms are errors. Collinear
arms are supported; a straight angle's arc uses a deterministic plane.

`connector vector vector "caption";` is a dashed leader with the caption at
the second endpoint. It does not infer a bond or chemical interaction. Each
vector may be three numbers or `position atom-selection`. A multi-atom
selection means its geometric centroid; an empty selection is an error.
For example, `res-atom A15 CA` addresses the PDB residue named `A15`, not `15`.

An optional scale and unit label convert distances:

```text
distance 0 0 0 3 4 0 scale 0.1 "nm";
```

This prints `0.50 nm`. Units are not inferred from the label: the positive
scale explicitly supplies the conversion. Coordinates remain Angstroms.

Text uses the current label size, colour, clipping and offset. Place labels
manually with `labeloffset`; no automatic collision avoidance is attempted.
Dash geometry is generated explicitly so that 3D outputs also show gaps.
The default dash length is 0.3 Å. A positive `linedash` sets that length using
the existing 0.04 Å-per-line-unit conversion. A limit of 20,000 dash intervals
rejects impractically dense patterns. These commands restore graphics state.

See [ligand measurements](examples/ras_measurements.svg) and its
[script](examples/ras_measurements.in).

### Unit symbols and fonts

Generated distance labels use `Å` (U+00C5, A with a ring), and MolAuto B-factor
legend titles use `Å²`. Script strings are UTF-8; for example:

```text
label 0 0 0 "Distance / Å";
legend "B-factor (Å²)" -16 -30 0 32 1.5 0 100 blue-white-red;
```

SVG, VRML, X3D and WebGL retain UTF-8 text. PostScript uses a Latin-1-encoded
Times font. MetaPost composes the ring and superscript as vector graphics,
without requiring TeX. OpenGL and its raster outputs support the symbols with
both bundled bitmap and optional FreeType fonts; Raster3D uses bundled stroke
geometry even in the basic build. See [output fonts](FONTS.md) for family
selection and font-file options. The alternative angstrom sign `Å` (U+212B) is
also supported. This is not a promise of general Unicode support in the legacy
fonts: PostScript is limited to Latin-1 and the bundled bitmap/stroke fonts to
ASCII plus these unit symbols. Avoid legacy Greek `labelmask` substitutions on
multibyte text.

POV-Ray still exports labels as comments, not visible text. Converting SVG to
PNG preserves the SVG text, independently of the OpenGL font setting.

## Backbones through C-alpha atoms

Use a thin `turn` to draw a smooth curve through every selected C-alpha atom:

```text
set coilradius 0, linewidth 1.2;
turn amino-acids;
```

`coil` performs Priestle smoothing first, so its curve normally **does not**
pass through the original C-alpha coordinates. Use `turn`, or explicitly set
`smoothsteps 0` before `coil`, when exact interpolation is wanted. Zero is now
an accepted smoothing count; the default remains two. This also disables
coordinate smoothing for strands, but does not turn their ribbon edges or
arrowheads into a C-alpha trace. `trace` remains straight atom-to-atom segments.

SVG, PostScript and MetaPost use exact cubic Bezier controls derived from the
original Hermite spline, rather than fitting another curve to sampled points.
Changing `segments` or `curvetolerance` subdivides the same thin backbone curve
without changing its shape, apart from output-coordinate rounding. Colour
boundaries retain the same tangents; chain breaks are never joined. Vector
depth sorting still splits paths, so intersection handling remains approximate.
Other backends retain sampled thin lines; solid tubes keep their own rendering.

The [thin backbone example](examples/ras_curved_lines.svg) now uses `turn`, with
small black spheres marking the C-alpha positions. Its
[script](examples/ras_curved_lines.in) is editable.

## Adaptive curve sampling

```text
set curvetolerance 0.02;
```

The tolerance is in Angstroms. Sampling is chosen separately for each cubic
backbone interval in coils, turns, double helices, helices and strand bodies.
The maximum norm of the cubic's second derivative bounds the centreline
polyline chord error by `M / (8*n*n)`. Straight intervals need fewer samples
than tightly curved ones. An even number of subdivisions retains categorical
colour boundaries at the interval midpoint.

`curvetolerance 0` is the default and retains the existing `segments` rule.
With a positive tolerance, that rule is replaced for these backbone intervals.
The minimum is two subdivisions; a requested tolerance requiring over 4,096
subdivisions per interval produces an error rather than silently being relaxed.
`segments` still controls other geometry, such as mesh spheres and cylinders.
`segmentsize` still controls vector depth subdivision.

The bound concerns the sampled centreline, not ribbon-edge rotation, tube
circumference, arrowheads, pixel coverage, vector depth sorting, or
POV-Ray spline fitting. It is not screen-space antialiasing and does not change
the molecular smoothing algorithm. Strand arrowheads retain their fixed layout.

MolAuto accepts `-curvetolerance 0.02` and emits the corresponding editable
state command.

## Continuous scalar colouring

```text
set colourparts on,
    residuecolour amino-acids b-factor 0 100 alphafold-gradient,
    colourinterpolate on;
```

`colourinterpolate on` interpolates normalized scalar values between adjacent
residues, then evaluates the original colour ramp at each sampled point. This
means blue-white-red passes through white, not purple. The ramp and range are
captured when `residuecolour ... b-factor` is set; later ramp changes cannot
silently recolour already assigned residues. Values are clamped to the range
before interpolation, matching the endpoint colours.

This affects coils, thin backbone curves, helices and strand bodies. Rendering
uses sampled colours, not an analytic surface gradient. Enabling interpolation
also imposes at least 16 subdivisions per interval for colour resolution.
Native helix cylinders remain residue-banded, and strand arrowheads retain
their existing categorical treatment.

The discrete `alphafold` ramp is never interpolated. Explicit residue colours,
secondary-structure palettes and sequence rainbows also keep hard boundaries.
Interpolation requires adjacent residues in the same molecule/chain with the
same scalar range and ramp. Reassigning an explicit colour clears its scalar
metadata. As with other copied molecular styling, set scalar colours after a
`copy` command if you want interpolation on the new molecule.

`colourinterpolate off` is the default. MolAuto accepts `-colourinterpolate`
with `-bfactor`, `-alphafold_gradient` or `-alphafold`; the last still retains
its four discrete bins. The option does not override `colourparts off`.

See [adaptive scalar cartoon](examples/ras_smooth_scalar.svg) and its
[script](examples/ras_smooth_scalar.in).

## Legend layouts and categories

The original legend command remains valid. Append `vertical` to run low-to-high
upwards. Append `ticks { value "label"; ... }` for explicit positions and text:

```text
legend "Confidence" 10 -15 0 1.8 28 0 100 alphafold-gradient vertical
  ticks { 0 "low"; 70 "confident"; 100 "high"; };
```

Width and height describe the bar, regardless of orientation. Tick positions
must increase strictly and lie within LOW and HIGH. An empty `ticks {}` hides
ticks and tick labels. Omit the clause to retain the existing automatic ticks.
Allow space to the right of a vertical bar for labels; text remains horizontal.
The optional clauses must appear in the order `vertical`, then `ticks`.

Categorical keys use explicit colour/caption pairs:

```text
key "Secondary structure" -22 -4 0 2 15 vertical {
  red "Helix";
  blue "Strand";
  grey 0.5 "Coil";
};
```

Omit `vertical` for left-to-right items. Vertical categories run top-to-bottom.
Width and height cover the swatch layout; gaps occupy 20% of each swatch cell.
Labels are below horizontal swatches and to the right of vertical ones.
Keys do not recolour the molecule: use the same colours in the drawing commands.
Each key must have at least one item. Both custom ticks and keys allow up to
256 items. These are scene geometry, not screen overlays.

See [layout examples](examples/legend_layouts.svg) and their
[script](examples/legend_layouts.in). Earlier legend details remain in
[ANNOTATIONS_AND_CHECKS.md](ANNOTATIONS_AND_CHECKS.md).

## Reusable views

Export a script's cumulative whole-molecule coordinate transform, area, window
and slab as ordinary MolScript macros:

```bash
./molscript -svg -in model.in -out model.svg -viewout model.view.inc
./molauto -nice -bfactor -view model.view.inc aligned-model.pdb > aligned-model.in
./molscript -svg -in aligned-model.in -out aligned-model.svg
```

The new file contains `view_header` and `view_transform`. It is editable text,
and existing files are deliberately not overwritten. Choose a new filename
when exporting another view. To reuse it manually:

```text
@"model.view.inc"
plot
  $view_header
  read mol "aligned-model.pdb";
  transform atom * $view_transform;
  coil amino-acids;
end_plot
```

Quoted include paths now support spaces. Paths remain relative to the working
directory, not to the referring script. Import the macros before `plot`; expand
the header before coordinate commands. Do not centre again: the saved
translation is already included. Related structures must be aligned in the
original coordinate frame. View reuse does not perform structural alignment.

`-viewout` requires one nonempty plot. It rejects partial-coordinate transforms,
transforms after drawing, and coordinate reads after transforms, since those
do not describe a single common frame. It cannot be combined with `-check`.
MolAuto `-view` replaces its usual centring and conflicts with `-window`,
`-rotate` and `-translate`. It references the supplied macro file rather than
embedding a second copy.

This exports the scripted view, not subsequent mouse navigation in OpenGL or
WebGL. It does not save lighting, labels, colours, perspective settings, or
backend-specific camera behaviour. Exact image framing requires the same
backend/aspect settings; `area` is meaningful for vector output, not every 3D
viewer. There is no hidden session state.

See [reused Ras view](examples/ras_reused_view.svg), its
[script](examples/ras_reused_view.in), and the
[saved macros](examples/ras_saved_view.inc).

## Tests and examples

```bash
python3 scripts/test-figure-tools.py
python3 scripts/generate-figure-examples.py
```

The generator writes four examples and their native scene formats and SVG PNG
previews. It deliberately replaces the generated `examples/ras_saved_view.inc`.
It requires CairoSVG only for previews; the C implementation has no new
external dependency.
