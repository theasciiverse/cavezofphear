# Changes from upstream

This is the Asciiverse packaging of [CAVEZ of PHEAR](https://github.com/AMDmi3/cavezofphear),
Tom Rune Flo's Boulder Dash-style cave game for the terminal (C, ncurses,
2003 to 2011), maintained since 2021 by Dmitry Marakasov. Upstream is the
`upstream` remote; the `asciiverse` branch carries the changes below.

Built from upstream `master` at `e2f9369` (2025-09-29), one CI-only commit
after the 0.6.2 tag.

## Licence

GPL-3.0-or-later: the GNU GPL 3 in LICENSE, and every source file says
"either version 3 of the License, or (at your option) any later version"
(checked 2026-11-02).

## Code

- **data/ beside the binary.** The data folder was a path fixed at build
  time (the source tree, or `/usr/share/phear` for a system-wide build), so
  a copy installed anywhere else could not start ("Unable to find data,
  aborting."). `data/` beside the running binary (`/proc/self/exe` on
  Linux, `_NSGetExecutablePath` on macOS) is now tried first, then the
  build-time folder, so one binary works from an av install and from a
  system package. The path buffers built from it are sized from PATH_MAX,
  so long install paths load. Written to be offered upstream; builds clean
  with upstream's CI flags (`-Werror -Wall -Wextra -pedantic`) under gcc
  and clang.
- **Ctrl+C, SIGTERM and SIGHUP quit cleanly.** Ctrl+C exited with status 1
  and "got SIGINT, cleaning up"; it now leaves like q, with the terminal put
  back and status 0. SIGTERM and SIGHUP were not handled and left the
  terminal raw; they now do the same, in the game and in the editor.

## Packaging

- Upstream's `ci.yml` (builds on every push and once a month) is removed.
- `.gitignore` (upstream has none) keeps a local `build/` folder out.
- `.github/workflows/release.yml`: av archives `cavezofphear-<target>` for
  Linux (static musl with ncurses linked in, x86-64 and ARM64, built in
  Alpine) and macOS (x86-64 and ARM64, the system ncurses), each holding
  `phear` with `data/` beside it, LICENSE, README and this file, plus
  SHA256SUMS; and .deb, .rpm, .apk and Arch packages of the static Linux
  binary by nfpm (`asciiverse/nfpm.yaml`): `/usr/bin/phear` with the data
  in `/usr/share/phear`, no menu entry. No Windows (POSIX signals,
  `nanosleep`). Runs on our tags only (`*-av.*`).
- `asciiverse/cavezofphear.cast`, the catalog preview, recorded at 100x30
  with hub's `tools/castrec.py`: the title screen, then cave 1 played for
  twelve diamonds along a route planned so no stone falls on the player
  (`asciiverse/plan-route.py`, run by `asciiverse/record-preview.sh`).

av starts it as `phear` with no arguments.

## Checked

- Linux: the static musl build (Alpine 3.22, CMake, ncurses 6 linked in,
  298 KB) started from a 326-character install path, with the working
  folder elsewhere, finds its data; with `data/` removed it stops with
  "Unable to find data, aborting."; Ctrl+C quits with status 0; q asks
  first and quits with status 0; resized from 1x1 to 120x40 without a
  crash, at about 2.6 MB. The .deb installed in a clean Debian container
  starts and finds `/usr/share/phear/data`.
- macOS: not built here; the release workflow's macOS job is the first
  build of the `_NSGetExecutablePath` branch.

## Ours

- **How to play it, at the top of README.md.** A short note on installing
  and starting it with av; the original text follows unchanged.
