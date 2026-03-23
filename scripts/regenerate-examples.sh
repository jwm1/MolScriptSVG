#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
EXAMPLES="$ROOT/examples"
MOLSCRIPT="$ROOT/molscript"

if [ ! -x "$MOLSCRIPT" ]; then
  echo "missing executable: $MOLSCRIPT" >&2
  echo "build MolScriptSVG first" >&2
  exit 1
fi

cd "$EXAMPLES"

for input in *.in; do
  base=${input%.in}
  echo "==> regenerating $base"
  "$MOLSCRIPT" -ps -in "$input" -out "$base.ps"
  "$MOLSCRIPT" -svg -in "$input" -out "$base.svg"
  "$MOLSCRIPT" -mp -in "$input" -out "$base.mp"
  "$MOLSCRIPT" -x3d -in "$input" -out "$base.x3d"
  "$MOLSCRIPT" -x3dv -in "$input" -out "$base.x3dv"
  "$MOLSCRIPT" -webgl -in "$input" -out "$base.webgl.html"
done

if command -v inkscape >/dev/null 2>&1; then
  echo "==> rasterizing SVG outputs"
  rm -f *_svg.png
  for svg in *.svg; do
    abs_svg="$EXAMPLES/$svg"
    base=${svg%.svg}
    abs_png="$EXAMPLES/${base}_svg.png"
    inkscape "$abs_svg" --export-type=png --export-filename="$abs_png" >/dev/null 2>&1
  done
else
  echo "warning: inkscape not found; skipping *_svg.png generation" >&2
fi

printf 'done: in=%s ps=%s svg=%s mp=%s x3d=%s x3dv=%s webgl=%s svg_png=%s\n' \
  "$(ls -1 *.in | wc -l)" \
  "$(ls -1 *.ps | wc -l)" \
  "$(ls -1 *.svg | wc -l)" \
  "$(ls -1 *.mp | wc -l)" \
  "$(ls -1 *.x3d | wc -l)" \
  "$(ls -1 *.x3dv | wc -l)" \
  "$(ls -1 *.webgl.html | wc -l)" \
  "$(ls -1 *_svg.png 2>/dev/null | wc -l)"
