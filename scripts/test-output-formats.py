#!/usr/bin/env python3
"""Check all example scene formats; --images also tests EGL image export."""
import argparse
import html
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
EXAMPLES = ROOT / "examples"
BINARY = str(ROOT / "molscript")


def run(args, cwd=EXAMPLES, env=None):
    proc = subprocess.run(args, cwd=cwd, env=env, capture_output=True, text=True, timeout=120)
    if proc.returncode:
        raise RuntimeError(f"{args}:\n{proc.stdout}\n{proc.stderr}")
    return proc


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--images", action="store_true")
    options = parser.parse_args()
    modes = {"ps": "-ps", "mp": "-mp", "svg": "-svg", "wrl": "-vrml",
             "x3d": "-x3d", "x3dv": "-x3dv", "webgl.html": "-webgl",
             "pov": "-pov", "r3d": "-raster3d"}
    count = 0
    with tempfile.TemporaryDirectory(prefix="molscript-formats-") as tmp:
        tmp = Path(tmp)
        for source in sorted(EXAMPLES.glob("*.in")):
            for suffix, mode in modes.items():
                target = tmp / f"{source.stem}.{suffix}"
                run([BINARY, mode, "-in", str(source), "-out", str(target)])
                if not target.stat().st_size:
                    raise RuntimeError(f"empty output: {target}")
                if suffix in ("svg", "x3d"):
                    ET.parse(target)
                elif suffix == "webgl.html":
                    embedded = re.search(r'<textarea id="molscript-scene-data">(.*?)</textarea>',
                                         target.read_text(), re.S)
                    if embedded is None:
                        raise RuntimeError(f"missing embedded scene: {target}")
                    ET.fromstring(html.unescape(embedded.group(1)))
                count += 1
            print(f"Generated 9 formats: {source.name}", flush=True)

        checks = {
            "gs": ["-q", "-dBATCH", "-dNOPAUSE", "-sDEVICE=nullpage", str(tmp / "ras_std.ps")],
            "mpost": ["-interaction=nonstopmode", "ras_std.mp"],
            "view3dscene": ["--write", str(tmp / "ras_std.x3d")],
            "povray": ["+I" + str(tmp / "ras_std.pov"), "+O" + str(tmp / "pov.png"),
                       "+FN", "+W200", "+H200", "-D"],
        }
        for program, args in checks.items():
            if shutil.which(program):
                env = dict(os.environ, HOME=str(tmp), XDG_CONFIG_HOME=str(tmp / "config"))
                run([program, *args], cwd=tmp, env=env)
                print(f"{program} accepted ras_std", flush=True)
            else:
                print(f"SKIP: {program} not installed")
        if options.images:
            from PIL import Image
            env = dict(os.environ)
            env.pop("DISPLAY", None)
            for suffix, flag in (("png", "-png"), ("jpg", "-jpeg"), ("gif", "-gif"), ("sgi", "-sgi")):
                target = tmp / f"image.{suffix}"
                run([BINARY, flag, "-size", "201", "199", "-in", "ras_std.in", "-out", str(target)], env=env)
                with Image.open(target) as im:
                    im.load()
                    if im.size != (201, 199) or all(lo == hi for lo, hi in im.convert("RGB").getextrema()):
                        raise RuntimeError(f"wrong dimensions or empty image: {target}")
                print(f"Headless {suffix}: 201 x 199, nonempty")
        print(f"Validated generation of {count} example outputs")


if __name__ == "__main__":
    main()
