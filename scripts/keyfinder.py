"""Detect the musical key of audio files with the kfcli/keyfinder.exe wrapper.

Decodes each file to float32 mono PCM (via soundfile) at its native sample rate,
pipes it to the executable, and prints "<key>\t<path>".

    python scripts/keyfinder.py track1.flac track2.wav
    KEYFINDER_EXE=path\to\keyfinder.exe python scripts/keyfinder.py *.flac

Requires: soundfile, numpy  (pip install soundfile numpy)
"""
import os
import subprocess
import sys
import tempfile

import numpy as np
import soundfile as sf


def detect_key(audio_path: str, exe: str) -> str:
    data, sr = sf.read(audio_path, dtype="float32", always_2d=True)
    mono = data.mean(axis=1).astype(np.float32)
    tmp = tempfile.mktemp(suffix=".f32")
    mono.tofile(tmp)
    try:
        r = subprocess.run([exe, tmp, str(sr)], capture_output=True, text=True, timeout=180)
        if r.returncode != 0:
            raise RuntimeError(r.stderr.strip() or f"exit {r.returncode}")
        return r.stdout.strip()
    finally:
        os.remove(tmp)


if __name__ == "__main__":
    default = os.path.join(os.path.dirname(__file__), "..", "dist", "keyfinder.exe")
    exe = os.environ.get("KEYFINDER_EXE", default)
    if not sys.argv[1:]:
        print(__doc__)
        sys.exit(0)
    for path in sys.argv[1:]:
        print(f"{detect_key(path, exe)}\t{path}")
