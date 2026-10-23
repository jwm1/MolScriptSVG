#!/bin/sh
# Archive a committed release, retaining generated examples but no build files.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT"
version=$(cat VERSION)
tag=${1:-v$version}
if [ "$tag" != "v$version" ]; then
  echo "expected tag v$version, got $tag" >&2
  exit 1
fi
if [ -n "$(git status --porcelain)" ]; then
  echo "commit the release changes before packaging" >&2
  exit 1
fi
if [ "$(git rev-parse "$tag^{commit}")" != "$(git rev-parse HEAD)" ]; then
  echo "HEAD must match the release tag" >&2
  exit 1
fi
mkdir -p dist
name="MolScriptSVG_$version"
git archive --format=tar.gz --prefix="$name/" \
  --output="dist/$name.tar.gz" "$tag"
(cd dist && sha256sum "$name.tar.gz" > SHA256SUMS)
printf 'Created dist/%s.tar.gz and dist/SHA256SUMS\n' "$name"
