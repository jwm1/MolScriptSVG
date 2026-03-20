#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
EXAMPLES="$ROOT/examples"
MOLSCRIPT="$ROOT/molscript"
SMOKE_FILES="
$EXAMPLES/smoke_ras_std.svg
$EXAMPLES/smoke_ras_std.x3d
$EXAMPLES/smoke_ras_std.x3dv
$EXAMPLES/smoke_ras_std.webgl.html
$EXAMPLES/smoke_ras_std_svg.png
"

if [ ! -x "$MOLSCRIPT" ]; then
  echo "missing executable: $MOLSCRIPT" >&2
  echo "build MolScript first" >&2
  exit 1
fi

cleanup() {
  rm -f $SMOKE_FILES /tmp/molscript_smoke_x3d.x3dv
}

trap cleanup EXIT INT TERM

run_case() {
  mode=$1
  input=$2
  output=$3

  echo "==> $mode $input -> $output"
  (
    cd "$EXAMPLES"
    "$MOLSCRIPT" "$mode" -in "$input" -out "$output"
  )
  test -s "$EXAMPLES/$output"
}

run_case -svg ras_std.in smoke_ras_std.svg
run_case -x3d ras_std.in smoke_ras_std.x3d
run_case -x3dv ras_std.in smoke_ras_std.x3dv
run_case -webgl ras_std.in smoke_ras_std.webgl.html
grep -q '<x3d-canvas id="molscript-scene"' "$EXAMPLES/smoke_ras_std.webgl.html"
grep -q 'src="data:model/x3d+xml;charset=utf-8,' "$EXAMPLES/smoke_ras_std.webgl.html"

if command -v inkscape >/dev/null 2>&1; then
  echo "==> rasterizing smoke_ras_std.svg"
  if inkscape "$EXAMPLES/smoke_ras_std.svg" --export-type=png \
      --export-filename="$EXAMPLES/smoke_ras_std_svg.png" >/dev/null 2>&1; then
    test -s "$EXAMPLES/smoke_ras_std_svg.png"
  else
    echo "warning: skipping inkscape validation" >&2
  fi
fi

if command -v view3dscene >/dev/null 2>&1; then
  echo "==> validating smoke_ras_std.x3d"
  if HOME=/tmp view3dscene --write "$EXAMPLES/smoke_ras_std.x3d" >/tmp/molscript_smoke_x3d.x3dv; then
    test -s /tmp/molscript_smoke_x3d.x3dv
    rm -f /tmp/molscript_smoke_x3d.x3dv
  else
    echo "warning: skipping view3dscene validation" >&2
  fi
fi

echo "smoke test ok"
