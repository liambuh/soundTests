SR=44100
NAME=$1
arecord -f S16_LS -c 1 -r $SR | sox -t raw -r $SR -e signed -b 16 -c 1 - -t raw -c 2 - | ./$NAME
