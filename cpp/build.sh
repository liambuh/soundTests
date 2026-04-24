#!/bin/bash
set -e

NAME="main.cpp"
OUT="app"

RUN=0
COMPILE=0

# parse flags:
while getopts "cr" opt; do
	case $opt in
		c)
			COMPILE=1
			;;
		r)
			RUN=1
			;;
		*)
			echo "Usage: $0 [-c] [-r]"
			exit 1
			;;
	esac
done

##compilation
if [[ $COMPILE -eq 1 ]]; then
	g++ "$NAME" audio.o -lasound -o "$OUT"
	echo "compiled"
fi

if [[ "$RUN" -eq 1 ]]; then
	echo "running.."
	./$OUT
fi
