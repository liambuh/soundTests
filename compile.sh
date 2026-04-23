#!/bin/bash
set -e

NAME="simple_recording.c"
OUT="recorder"

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
	gcc "$NAME" -lasound -o "$OUT"
	echo "compiled"
fi

if [[ "$RUN" -eq 1 ]]; then
	echo "running.."
	arecord -D plughw:0,6 -f S16_LE -c 1 -r 44100 | ./$OUT
fi
