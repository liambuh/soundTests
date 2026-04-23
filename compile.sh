#!/bin/bash
set -e

NAME="simple_recording.c"
OUT="recorder"
RUN=$1

gcc "$NAME" -lasound -o "$OUT"

echo "compiled"

if [[ "$RUN" == "run" ]]; then
	echo "running.."
	arecord -D plughw:0,6 -f S16_LE -c 1 -r 44100 | ./$OUT
fi
