# B-factor and confidence colours (2.3)

Run MolAuto with one of `-bfactor`, `-alphafold`, `-alphafold_gradient`.
All colour protein/nucleotide cartoons and individual ligand atoms. Protein
values come from CA, falling back to an atom average; nucleotides use averages.
Waters and omitted ligands are excluded from automatic range calculation.

`-bfactor` maps the displayed minimum to blue, midpoint to white and maximum
to red. Override with `-bfactor_range LOW HIGH`. Constant data map to white.
`-alphafold` assumes the B column contains confidence on a fixed 0-100 scale:

| Range | RGB (0-255) |
| --- | --- |
| <=50 | 238,132,83 (orange) |
| >50 to 70 | 249,220,77 (yellow) |
| >70 to 90 | 117,199,244 (cyan) |
| >90 | 33,82,206 (blue) |

Colours and strict boundaries match James Murray's `afcolors.py`, whose
colour matching is credited to Daniella Pretorius.
The custom continuous variant interpolates RGB at 0/orange, 50/yellow,
70/cyan, 100/blue. It is not the standard four-bin AlphaFold scheme.
Out-of-range confidence is clamped with a warning. Non-finite scores are errors.
Blank PDB B-factor fields retain the reader's existing zero interpretation.

The modes are mutually exclusive and conflict with `-nocolour`, `-ss_palette`
and `-colourblind`. `-nice` and `-publication` retain their geometry settings;
the scalar palette takes precedence. `-noligand` omits ligand colouring.
By default, continuous refers to the value-to-colour mapping, not interpolation
between residues. Optional `colourinterpolate on` adds sampled interpolation between adjacent
scalar-coloured residues; categorical palettes remain discrete. See
[figure tools](FIGURE_TOOLS.md) for controls and limitations.

Native MolScript also accepts the named ramps `blue-white-red`, `alphafold`
and `alphafold-gradient`, for example:

```text
set colourparts on,
    residuecolour amino-acids b-factor 0 100 alphafold,
    atomcolour not in waters b-factor 0 100 alphafold;
```

Named ramps use normalized endpoints; use 0 and 100 for confidence. Existing
two-colour and rainbow ramps are unchanged. With `colourparts on`, cylinders
now use residue-coloured axial bands, merging equal neighbours. The shared
X3D/WebGL and VRML use native Cylinder nodes with only terminal caps; POV-Ray
uses native open cylinders with terminal discs. Other backends use meshes,
with radial normals for 3D shading and flat facet normals for vector output;
with `colourparts off`, the old cylinder path is unchanged.

Round coils/turns now use 16-sided circular rings with radial normals in
OpenGL, Raster3D, VRML and X3D. Cap normals remain separate, so the flat ends
are not rounded by shading. POV-Ray uses local cubic `sphere_sweep` spans,
joined at matching tangent-plane cuts to retain colour boundaries without
clipping distant parts of a folded loop. Thin coils (`coilradius 0`) use
piecewise cubic Bezier paths in SVG, PS and MetaPost. Explicit lines, bonds
and unsmoothed traces remain straight. Beziers are subdivided using
`segmentsize` before the existing depth sort; this retains painter-style
occlusion approximations, not exact intersection clipping. Colour and chain
breaks remain separate paths. Vector controls now preserve the exact original
Hermite spline. Use a thin `turn` to pass through C-alpha coordinates;
`coil` smooths those coordinates first unless `smoothsteps 0` is set.
See [backbones and sampling](FIGURE_TOOLS.md#backbones-through-c-alpha-atoms).

## Examples and tests

Use `-legend X Y Z WIDTH HEIGHT` with a scalar mode to generate a matching
key with its exact numeric range. See [legends and checks](ANNOTATIONS_AND_CHECKS.md).
The B-factor key is titled `B-factor (Å²)`; confidence keys remain dimensionless.
The generator also creates three `*_legend` examples and PNG previews.

From the repository root:

```bash
make -C code -j4 USE_FREETYPE=1
python3 scripts/test-scalar-colours.py
python3 scripts/generate-scalar-examples.py
```

The generator requires CairoSVG and writes scripts, scenes and PNG previews
to `examples/`. `ras_bfactor` uses the original B-factors. The three
`ras_confidence*` examples use explicitly synthetic scores, not a prediction.
Open `examples/index.html` to compare them.

Modern Bison is required to regenerate the parser. Checked-in generated
sources are supplied. This development tree has no new release tag.
