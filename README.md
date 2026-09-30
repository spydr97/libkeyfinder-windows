# keyfinder.exe — libKeyFinder on Windows

A small, **static, dependency-free** Windows command-line wrapper around
[libKeyFinder](https://github.com/mixxxdj/libkeyfinder) (the musical-key detector
used by Mixxx). It exists because there is no current prebuilt Windows binary of
libKeyFinder / `keyfinder-cli`, and building it by hand (FFTW + MSVC) is fiddly.

This repo gives you a **one-command reproducible build** (vcpkg + MSVC) and a
**CI workflow that publishes the `.exe` as a GitHub release** on every tag.

## What it does

`kfcli` reads raw **32-bit float, mono, little-endian PCM** and prints the key:

```
> keyfinder.exe track.f32 44100
D minor
```

Raw PCM (not WAV/FLAC) keeps the C++ tiny and lets your existing decode pipeline
feed it directly. Use the Python helper for real audio files:

```
pip install soundfile numpy
python scripts/keyfinder.py mytrack.flac
# -> "D minor	mytrack.flac"
```

Output keys use libKeyFinder's spellings (flats: `Bb`, `Db`, `Eb`, `Gb`, `Ab`).

## Build it yourself

**Requirements:** Windows, Visual Studio 2019/2022 (or Build Tools) with
"Desktop development with C++", and [vcpkg](https://github.com/microsoft/vcpkg).

```bat
git clone https://github.com/microsoft/vcpkg %USERPROFILE%\vcpkg
%USERPROFILE%\vcpkg\bootstrap-vcpkg.bat
build.bat
```

`build.bat` locates Visual Studio via `vswhere`, uses the CMake/Ninja that ship
with it, provisions libKeyFinder + FFTW through vcpkg (manifest mode, pinned by
`builtin-baseline` in `vcpkg.json`), links the static CRT (`/MT`), and drops the
result at `dist\keyfinder.exe`. No DLLs to ship.

Set `VCPKG_ROOT` if your vcpkg lives elsewhere.

## Get the prebuilt binary

Grab `keyfinder.exe` from the [Releases](../../releases) page. Each release is
built by GitHub Actions (`.github/workflows/release.yml`) from this exact source
— push a tag like `v1.0.0` (or run the workflow manually) to cut a new one.

## Layout

| Path | Purpose |
|---|---|
| `src/kfcli.cpp` | the wrapper (raw PCM → libKeyFinder → key) |
| `CMakeLists.txt` | build definition (static CRT, links `KeyFinder::keyfinder`) |
| `vcpkg.json` | dependency manifest, pinned baseline |
| `build.bat` | one-command local Windows build |
| `scripts/keyfinder.py` | decode any audio file → key |
| `.github/workflows/release.yml` | CI build + release on tag |

## License

**GPL-3.0-or-later** — this wrapper links GPL-3.0 libKeyFinder, so any distributed
binary is GPL-3.0. See [LICENSE](LICENSE) and [NOTICE.md](NOTICE.md) for full
attribution (libKeyFinder, FFTW).
