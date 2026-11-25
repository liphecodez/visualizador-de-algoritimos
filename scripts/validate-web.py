"""Validate the actual static export, including WASM and JavaScript syntax."""
import argparse
from functools import partial
from html.parser import HTMLParser
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import shutil
import subprocess
import threading
from urllib.request import urlopen


class ShellParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.scripts = []
        self.canvas = None

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "script" and "src" in attrs:
            self.scripts.append(attrs["src"])
        if tag == "canvas" and attrs.get("id") == "canvas":
            self.canvas = attrs


def validate(directory):
    expected = {"index.html", "index.js", "index.wasm", ".nojekyll", "LICENSE-raylib.txt"}
    if {p.name for p in directory.iterdir()} != expected:
        raise RuntimeError("Unexpected or missing files in the static export")
    for name in expected - {".nojekyll"}:
        if not (directory / name).stat().st_size:
            raise RuntimeError(f"Empty artifact: {name}")
    shell = ShellParser()
    shell.feed((directory / "index.html").read_text(encoding="utf-8-sig"))
    if shell.scripts != ["index.js"]:
        raise RuntimeError("The shell must reference only the relative index.js script")
    if shell.canvas is None or (shell.canvas.get("width"), shell.canvas.get("height")) != ("1100", "740"):
        raise RuntimeError("Missing canvas or incorrect logical dimensions")
    if (directory / "index.wasm").read_bytes()[:8] != b"\x00asm\x01\x00\x00\x00":
        raise RuntimeError("Invalid WebAssembly header")
    node = shutil.which("node")
    if node is None:
        raise RuntimeError("Node.js is required to validate JavaScript and WebAssembly")
    subprocess.run([node, "--check", str(directory / "index.js")], check=True)
    subprocess.run([node, "-e", "const fs=require('fs');if(!WebAssembly.validate(fs.readFileSync(process.argv[1])))process.exit(1)", str(directory / "index.wasm")], check=True)
    handler = partial(SimpleHTTPRequestHandler, directory=str(directory))
    server = ThreadingHTTPServer(("127.0.0.1", 0), handler)
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    try:
        base = f"http://127.0.0.1:{server.server_port}/"
        for name, mime in (("index.html", "text/html"), ("index.js", None), ("index.wasm", "application/wasm")):
            with urlopen(base + name, timeout=10) as response:
                content_type = response.headers.get_content_type()
                if response.status != 200 or response.read() != (directory / name).read_bytes():
                    raise RuntimeError(f"Incorrect static response: {name}")
                if mime and content_type != mime:
                    raise RuntimeError(f"Incorrect MIME type for {name}: {content_type}")
                if name.endswith(".js") and content_type not in {"text/javascript", "application/javascript"}:
                    raise RuntimeError(f"Incorrect JavaScript MIME type: {content_type}")
    finally:
        server.shutdown()
        thread.join(timeout=10)
        server.server_close()
    print("PASS: static export, relative script, canvas size, JavaScript syntax, WebAssembly binary and HTTP responses")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", nargs="?", default="dist/web", type=Path)
    validate(parser.parse_args().directory.resolve())
