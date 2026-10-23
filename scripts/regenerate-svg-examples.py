#!/usr/bin/env python3
"""Regenerate example SVGs and PNG previews, without deleting old previews first."""

import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rasterize-only", action="store_true")
    args = parser.parse_args()
    try:
        import cairosvg
    except ImportError:
        cairosvg = None
    inkscape = shutil.which("inkscape")
    if cairosvg is None and inkscape is None:
        parser.error("PNG rendering requires CairoSVG or Inkscape")

    examples = ROOT / "examples"
    inputs = sorted(examples.glob("*.in"))
    # Stage each pair on the same filesystem; only replace it after validation.
    with tempfile.TemporaryDirectory(prefix=".svg-render-", dir=examples) as tmp:
        for source in inputs:
            staged_svg = Path(tmp) / (source.stem + ".svg")
            staged_png = Path(tmp) / (source.stem + "_svg.png")
            destination = examples / staged_svg.name
            if args.rasterize_only:
                shutil.copyfile(destination, staged_svg)
            else:
                result = subprocess.run(
                    [str(ROOT / "molscript"), "-svg", "-in", source.name,
                     "-out", str(staged_svg)], cwd=examples,
                    capture_output=True, text=True, timeout=120,
                )
                if result.returncode:
                    raise RuntimeError(f"{source.name}: {result.stderr}")
            ET.parse(staged_svg)
            if cairosvg is not None:
                cairosvg.svg2png(url=str(staged_svg), write_to=str(staged_png))
            else:
                subprocess.run(
                    [inkscape, str(staged_svg), "--export-type=png",
                     "--export-filename=" + str(staged_png)], check=True, timeout=120,
                )
            with staged_png.open("rb") as handle:
                if handle.read(8) != b"\x89PNG\r\n\x1a\n":
                    raise RuntimeError(f"Invalid PNG for {source.name}")
            if not args.rasterize_only:
                staged_svg.replace(destination)
            staged_png.replace(examples / staged_png.name)
            print(f"refreshed {source.stem}.svg / {source.stem}_svg.png", flush=True)
    print(f"Validated {len(inputs)} SVG/PNG pairs")


if __name__ == "__main__":
    main()
