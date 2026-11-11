# Releasing MolScriptSVG 2.2

Historical 2.2 instructions. This tree is now unreleased 2.3 development;
do not run the 2.2 publication commands from this directory.

The prepared release lives in `MolScriptSVG_2.2`, on branch `main`, with the
original Git history. Its remote is `git@github.com:jwm1/MolScriptSVG.git`.
The executables remain `molscript` and `molauto`. `VERSION`, the application
banner, documentation headings and generated scene metadata use `2.2`.
Historical changelog entries and upstream MolScript/MolAuto version references
are intentionally retained.

## Validate and package

Run the regression commands in [RENDERING_TESTS.md](RENDERING_TESTS.md), inspect
the example gallery, and review `git diff --check`. GitHub Actions tests basic,
bitmap and FreeType builds on Ubuntu, including Xvfb/xdotool interaction.
Network-dependent browser tests are opt-in locally, not required by CI.
The new workflow must still pass on GitHub before publication.

The local annotated tag is `v2.2`. The packaging helper requires a clean tree
and HEAD at that tag. It creates a source archive and checksum in `dist/`;
generated examples are included, executables and object files are not.

```bash
sh scripts/package-release.sh
cd dist
sha256sum -c SHA256SUMS
```

## Push and create a draft

Run from the release directory after local validation. The ancestry check
stops if GitHub has commits absent locally; do not force-push. Fetching can
ask for your SSH passphrase. Authenticate `gh` separately if necessary.

```bash
set -e
git fetch origin
git merge-base --is-ancestor origin/main main
git push -u origin main
git push origin v2.2
gh release create v2.2 \
  dist/MolScriptSVG_2.2.tar.gz dist/SHA256SUMS \
  --repo jwm1/MolScriptSVG --verify-tag --draft \
  --title "MolScriptSVG 2.2" --notes-file RELEASE_NOTES_2.2.md
```

If the ancestry check fails, inspect `git log --oneline --left-right main...origin/main`
and merge the remote work deliberately. Rerun tests after resolving conflicts.
If that changes the intended release, recreate the unpublished local tag and
archive before pushing. Do not move a published release tag.

Review the draft and CI results in GitHub before clicking **Publish release**.
Alternatively use the GitHub Releases page, select the pushed `v2.2` tag,
paste `RELEASE_NOTES_2.2.md`, attach the archive and `SHA256SUMS`, and save a draft.
No push or GitHub release publication is performed by the packaging script.

The draft command follows the [GitHub CLI release documentation](https://cli.github.com/manual/gh_release_create).
