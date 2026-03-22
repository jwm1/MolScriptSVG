# Undocumented Features

This note compares the actual yacc grammar in
[code/molscript.y](./code/molscript.y) against the published syntax summary:

- <https://pekrau.github.io/MolScript/syntax.html>

That syntax page explicitly says it omits undocumented features. The items
below are supported by the grammar but are absent from that summary, or are
only represented there by an undefined placeholder production.

## Clearly Missing Commands

- `trace residue_selection ;`
  - Draws a peptide C-alpha trace as a polyline.
  - Grammar: `geom_command` in [code/molscript.y](./code/molscript.y)

- `element id`
  - Atom-selection primitive for selecting atoms by element symbol.
  - Grammar: `atom_specification` in [code/molscript.y](./code/molscript.y)

## Grammar Placeholders Not Expanded in the Published Summary

The syntax page uses these names, but does not define them:

- `xform`
- `parameter_spec`
- `vector`
- `colour`
- `ramp`
- `view_definition`

These omissions hide a number of real language features.

## Undocumented `transform ... by ...` Forms

The published summary includes:

- `transform atom_selection {by xform} ;`

but it does not document the supported `xform` forms:

- `centre vector`
- `translation vector`
- `rotation xaxis number`
- `rotation yaxis number`
- `rotation zaxis number`
- `rotation axis number number number number`
- `rotation` followed by a 3x3 matrix as 9 numbers
- `recall-matrix`

See `xform` in [code/molscript.y](./code/molscript.y).

## Undocumented `set` Parameters

The published summary includes:

- `set parameter_spec {',' parameter_spec} ';'`

but it does not document the concrete `parameter_spec` alternatives. These
include at least:

- `atomcolour atom_selection colour`
- `atomcolour atom_selection b-factor number number ramp`
- `atomradius atom_selection number`
- `bonddistance number`
- `bondcross number`
- `coilradius number`
- `colourparts on|off`
- `colourramp hsb|rgb`
- `cylinderradius number`
- `depthcue number`
- `emissivecolour colour`
- `helixthickness number`
- `helixwidth number`
- `hsbrampreverse on|off`
- `labelbackground number`
- `labelcentre on|off`
- `labelclip on|off`
- `labelmask id`
- `labeloffset vector`
- `labelrotation on|off`
- `labelsize number`
- `lightambientintensity number`
- `lightattenuation vector`
- `lightcolour colour`
- `lightintensity number`
- `lightradius number`
- `linecolour colour`
- `linedash number`
- `linewidth number`
- `objecttransform on|off`
- `planecolour colour`
- `plane2colour colour`
- `regularexpression on|off`
- `residuecolour residue_selection colour`
- `residuecolour residue_selection b-factor number number ramp`
- `residuecolour residue_selection ramp`
- `segments integer`
- `segmentsize number`
- `shading number`
- `shadingexponent number`
- `shininess number`
- `smoothsteps integer`
- `specularcolour colour`
- `splinefactor number`
- `stickradius number`
- `sticktaper number`
- `strandthickness number`
- `strandwidth number`
- `transparency number`

See `state_change` in [code/molscript.y](./code/molscript.y).

## Omitted Helper Syntax

The grammar also defines helper syntax that matters in practice but is not
shown on the summary page:

- `vector`
  - `position atom_selection`
  - `number number number`

- `colour`
  - `rgb number number number`
  - `hsb number number number`
  - `grey number`
  - named colour / identifier

- `ramp`
  - `from colour to colour`
  - `rainbow`

- `view_definition`
  - `from vector to vector`
  - `from vector to vector number`
  - `origin vector number`

- `number_as_id`
  - residue ranges accept either a numeric token or an identifier token at
    each end of `from ... to ...`

## Notes

- Most of the control-structure features such as `anchor`, `level-of-detail`,
  `viewpoint`, and the light commands are already present on the published
  syntax summary.
- The summary is therefore fairly complete at the top level; the largest gaps
  are the missing `trace` and `element` productions, and the omission of the
  full subgrammars behind `xform` and `parameter_spec`.
