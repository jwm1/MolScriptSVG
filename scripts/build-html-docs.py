#!/usr/bin/env python3
"""Build local HTML companions to root Markdown docs and check HTML navigation.

Generation requires Python-Markdown (python3-markdown on Debian/Ubuntu).
--check uses only the standard library and does not modify files.
"""
import argparse
from html import escape, unescape
from html.parser import HTMLParser
from pathlib import Path
import re
from urllib.parse import unquote, urlsplit, urlunsplit

ROOT = Path(__file__).resolve().parents[1]
HREF = re.compile(r'''(\bhref\s*=\s*["'])([^"']+)(["'])''', re.IGNORECASE)


def html_links(text):
    def replace(match):
        parts = urlsplit(unescape(match[2]))
        if not parts.scheme and not parts.netloc and parts.path.lower().endswith(".md"):
            path = parts.path[:-3] + ".html"
            url = urlunsplit((parts.scheme, parts.netloc, path, parts.query, parts.fragment))
            return match[1] + escape(url, quote=True) + match[3]
        return match[0]
    return HREF.sub(replace, text)


def documents():
    return sorted([*ROOT.glob("*.html"), *(ROOT / "docs").rglob("*.html"),
                   *(p for p in (ROOT / "examples").glob("*.html")
                     if not p.name.endswith(".webgl.html"))])


class Links(HTMLParser):
    def __init__(self, text):
        super().__init__()
        self.hrefs = []
        self.anchors = set()
        self.feed(text)

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "a" and "href" in attrs:
            self.hrefs.append(attrs["href"])
        for key in ("id", "name"):
            if attrs.get(key):
                self.anchors.add(attrs[key])


def check():
    errors = []
    parsed = {p.resolve(): Links(p.read_text(encoding="utf-8")) for p in documents()}
    for source, links in parsed.items():
        for href in links.hrefs:
            url = urlsplit(href)
            if url.path.lower().endswith(".md"):
                errors.append(f"{source.relative_to(ROOT)}: Markdown link {href}")
            if url.scheme in ("http", "ftp"):
                errors.append(f"{source.relative_to(ROOT)}: obsolete non-HTTPS link {href}")
            if url.scheme or url.netloc:
                continue
            path = (source.parent / unquote(url.path)).resolve() if url.path else source
            if path.suffix.lower() != ".html":
                continue  # Downloads, images and script sources are not documentation pages.
            if not path.is_file():
                errors.append(f"{source.relative_to(ROOT)}: missing HTML target {href}")
            # New generated documents have predictable Markdown heading anchors.
            elif path in parsed and path.with_suffix(".md").is_file() and url.fragment:
                if unquote(url.fragment) not in parsed[path].anchors:
                    errors.append(f"{source.relative_to(ROOT)}: missing heading {href}")
    for source in ROOT.glob("*.md"):
        if not source.with_suffix(".html").is_file():
            errors.append(f"missing HTML companion for {source.name}")
    if errors:
        raise SystemExit("\n".join(errors))
    print(f"Checked {len(parsed)} HTML pages: no Markdown links or missing local HTML targets.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="check links without regenerating")
    args = parser.parse_args()
    if not args.check:
        try:
            import markdown
        except ImportError:
            raise SystemExit("HTML generation requires Python-Markdown (python3-markdown).")
        for source in sorted(ROOT.glob("*.md")):
            body = markdown.markdown(source.read_text(encoding="utf-8"),
                                     extensions=["fenced_code", "tables", "toc", "sane_lists"])
            text = f'''<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MolScriptSVG: {escape(source.stem.replace('_', ' '))}</title>
<link rel="stylesheet" href="docs/style.css">
<link rel="stylesheet" href="docs/documentation.css">
</head>
<body>
<!-- Generated from {source.name} by scripts/build-html-docs.py. -->
<nav><a href="docs/index.html">Manual</a> | <a href="README.html">README</a> |
<a href="examples/index.html">Examples</a> | <a href="CHANGES.html">Changelog</a></nav>
<main>
{body}
</main>
</body>
</html>
'''
            source.with_suffix(".html").write_text(html_links(text), encoding="utf-8")
        for path in documents():
            old = path.read_text(encoding="utf-8")
            new = html_links(old)
            if new != old:
                path.write_text(new, encoding="utf-8")
    check()


if __name__ == "__main__":
    main()
