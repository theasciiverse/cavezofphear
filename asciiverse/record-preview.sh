#!/bin/sh
# Records asciiverse/cavezofphear.cast, the catalog preview, at 100x30: the
# title screen, then cave 1 along the route from plan-route.py, then q and
# y. Moves are sent as 8, 2, 4, 6: castrec sends normal-mode arrow escapes,
# which ncurses does not take here.
# Usage: asciiverse/record-preview.sh <folder with phear and data/> [castrec.py]
set -e
dir=$1
castrec=${2:-../../hub/tools/castrec.py}
here=$(dirname "$0")
script="w3.5;kspace;w1.2"
for k in $("$here/plan-route.py" "$here/../data/levels/01" 12 | tail -1); do
	case $k in Up) c=8 ;; Down) c=2 ;; Left) c=4 ;; Right) c=6 ;; esac
	script="$script;k$c;w0.2"
done
script="$script;w2;kq;w1;ky;w0.5"
python3 "$castrec" "$here/cavezofphear.cast" "$dir" "$script" -- ./phear
