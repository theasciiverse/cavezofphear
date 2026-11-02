# CAVEZ of PHEAR (Asciiverse packaging)

Tom Rune Flo's Boulder Dash-style cave game (C, ncurses, 2003 to 2011),
maintained by Dmitry Marakasov (AMDmi3) since 2021, GPL-3.0-or-later;
catalog id `cavezofphear`, binary `phear`. Not our code: keep changes
small, list every one in PATCHES.md, and offer fixes upstream.

- Remotes: `upstream` is github.com/AMDmi3/cavezofphear (branch `master`),
  `origin` is github.com/theasciiverse/cavezofphear.
- Branch `asciiverse` = upstream master plus our commits.
- Release: tag `v<version>-av.<n>` (now `v0.6.2-av.1`) and push only that
  tag; the workflow runs on `*-av.*` tags and also makes .deb, .rpm, .apk
  and Arch packages (`asciiverse/nfpm.yaml`).
- Data: `data/` beside the binary first, then the build-time `DATADIR`
  (`/usr/share/phear` for the release builds, `-DSYSTEMWIDE=ON
  -DCMAKE_INSTALL_PREFIX=/usr`).
- Local build: `cmake -S . -B build -DWITH_MANPAGE=OFF && cmake --build
  build`. Static: see the workflow; FindCurses picks the shared ncurses, so
  the static libraries are named by hand.
- Needs 80x25. Movement is the arrow keys or 8, 2, 4, 6.
- Preview: castrec sends normal-mode arrow escapes, which ncurses here does
  not take, so the recording moves with 8, 2, 4, 6. The route is planned
  by a script that digs to the nearest diamond and never steps under a
  stone that will fall (`asciiverse/plan-route.py`, used by `asciiverse/record-preview.sh`); cave 1
  played for twelve diamonds, all lives kept. pyte cannot render this cast
  (ncurses uses REP, `CSI b`); check frames with `asciinema convert -f txt`
  on a cut copy instead, which uses avt like the website and av.
- Run tests under a memory cap: `systemd-run --user --scope -p
  MemoryMax=1G ...`.
