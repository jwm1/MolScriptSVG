#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
EXAMPLES="$ROOT/examples"

if ! command -v povray >/dev/null 2>&1; then
  echo "missing povray executable" >&2
  exit 1
fi

mkdir -p /tmp/povray_home

cd "$EXAMPLES"
rm -f *_pov.png

for input in *.in; do
  base=${input%.in}
  pov="${base}.pov"
  png="${base}_pov.png"
  echo "==> rendering $pov -> $png"
  HOME=/tmp/povray_home povray \
    "+I$pov" "+O$png" +FN +W900 +H900 +A0.2 +R3 -D >/dev/null 2>&1
done

printf 'done: pov=%s pov_png=%s\n' \
  "$(ls -1 *.in | wc -l)" \
  "$(ls -1 *_pov.png | wc -l)"
