# Third-party components & attribution

`kfcli` / `keyfinder.exe` is a thin wrapper. The musical-key detection is done
entirely by **libKeyFinder**, and the build links **FFTW**. Because libKeyFinder
is GPL-3.0, any distributed binary of this wrapper is GPL-3.0-or-later (see
[LICENSE](LICENSE)).

## libKeyFinder
- Musical key detection library by Ibrahim Sha'ath, maintained by the Mixxx project.
- Source: https://github.com/mixxxdj/libkeyfinder
- License: GPL-3.0-or-later
- Consumed here via the vcpkg `libkeyfinder` port (v2.2.8).

## FFTW
- "Fastest Fourier Transform in the West", by Matteo Frigo and Steven G. Johnson.
- Source: https://www.fftw.org/
- License: GPL-2.0-or-later (a commercial non-GPL license is available from MIT).
- Consumed here via the vcpkg `fftw3` port.

## Build tooling
- Microsoft vcpkg (dependency provisioning) — MIT.
- Reference profiles / key spellings follow libKeyFinder's `KeyFinder::key_t`.
