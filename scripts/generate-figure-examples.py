#!/usr/bin/env python3
"""Regenerate figure-tool demonstrations and their scene/SVG PNG outputs."""
from pathlib import Path
import subprocess
import cairosvg

ROOT = Path(__file__).resolve().parents[1]
EXAMPLES = ROOT / "examples"
MOL = str(ROOT / "molscript")
AUTO = str(ROOT / "molauto")


def render(name, scene):
    (EXAMPLES / f"{name}.in").write_text(scene)
    for mode, extension in [("-svg", "svg"), ("-ps", "ps"), ("-mp", "mp"),
                            ("-pov", "pov"), ("-vrml", "wrl"), ("-x3d", "x3d"),
                            ("-x3dv", "x3dv"), ("-webgl", "webgl.html")]:
        subprocess.run([MOL, "-s", mode, "-in", f"{name}.in", "-out", f"{name}.{extension}"],
                       cwd=EXAMPLES, check=True)
    cairosvg.svg2png(url=str(EXAMPLES / f"{name}.svg"),
                    write_to=str(EXAMPLES / f"{name}_svg.png"))
    print(name, flush=True)


prefix = (EXAMPLES / "ras_gdp.in").read_text().split("  set planecolour", 1)[0]
prefix = prefix.replace("window 20;", "window 24;").replace("by translation -0.8 0 0", "by translation 1.5 0 0")
render("ras_measurements", prefix + '''
  set colourparts on, atomcolour atom * black, atomcolour element O red,
      atomcolour element N blue, atomcolour element P goldenrod,
      atomcolour element Mg green, labelsize 12, linecolour black,
      linewidth 0.8, depthcue 0, segments 8;
  ball-and-stick in residue 200;
  ball-and-stick in residue 201;
  ball-and-stick require not peptide and in residue 17;
  set linecolour black, labeloffset -0.7 -2 0;
  distance position res-atom 200 O1B position res-atom 201 MG;
  set labeloffset 0.8 1.8 0;
  distance position res-atom 17 OG position res-atom 201 MG;
  set labeloffset -2 0 0;
  angle position res-atom 200 O1B position res-atom 201 MG position res-atom 17 OG;
  set labeloffset 0 0.5 0;
  connector position res-atom 200 N2 2 5 0 "GDP";
end_plot
''')

scene = subprocess.check_output([AUTO, "-nice", "-stick", "-bfactor", "-colourinterpolate",
    "-curvetolerance", ".02", "-legend", "-16", "-30", "0", "32", "1.5", "ras.pdb"],
    cwd=EXAMPLES, text=True)
render("ras_smooth_scalar", scene)

render("legend_layouts", '''! Layout controls are explicit, in image-frame Angstroms.
plot
  area 0 0 640 500;
  window 52;
  set labelsize 13, linecolour black;
  legend "Confidence" 10 -15 0 1.8 28 0 100 alphafold-gradient vertical
    ticks { 0 "0: low"; 50 "50"; 70 "70: confident"; 100 "100: high"; };
  key "Secondary structure" -22 -4 0 2 15 vertical {
    rgb 0.85 0.35 0.15 "Helix";
    rgb 0.15 0.45 0.75 "Strand";
    grey 0.5 "Coil";
  };
  key "Chains" -22 -18 0 23 1.5 { red "A"; blue "B"; green "C"; };
end_plot
''')

# This particular file is a generated example, not a user view file.
view = EXAMPLES / "ras_saved_view.inc"
view.unlink(missing_ok=True)
subprocess.run([MOL, "-s", "-svg", "-in", "ras_std.in", "-viewout", view.name],
               cwd=EXAMPLES, check=True, stdout=subprocess.DEVNULL)
scene = subprocess.check_output([AUTO, "-nice", "-stick", "-bfactor", "-view", view.name,
                                 "ras.pdb"], cwd=EXAMPLES, text=True)
render("ras_reused_view", scene)
