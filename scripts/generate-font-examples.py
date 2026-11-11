#!/usr/bin/env python3
"""Render the same labels with the three portable vector font presets."""
from pathlib import Path
import subprocess
import cairosvg

ROOT = Path(__file__).resolve().parents[1]
EXAMPLES = ROOT / "examples"
for family in ("default", "serif", "sans", "monospace"):
    name = "font_labels" + ("" if family == "default" else "_" + family)
    modes = [("-svg", "svg"), ("-ps", "ps"), ("-mp", "mp"), ("-vrml", "wrl"),
             ("-x3d", "x3d"), ("-x3dv", "x3dv"), ("-webgl", "webgl.html")]
    if family == "default":
        modes += [("-pov", "pov"), ("-raster3d", "r3d")]
    for mode, extension in modes:
        subprocess.run([str(ROOT / "molscript"), "-s", mode, "-font", family,
                        "-in", "font_labels.in", "-out", f"{name}.{extension}"],
                       cwd=EXAMPLES, check=True)
    cairosvg.svg2png(url=str(EXAMPLES / f"{name}.svg"), write_to=str(EXAMPLES / f"{name}_svg.png"))
    print(name)
