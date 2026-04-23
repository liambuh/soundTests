Hello!

I am messing around with audio streaming and processing, and have made very minimal adjustments to a wonderful recording program found at the following link: https://www.linuxjournal.com/article/6735

Setup/Prerequisits:
To develop and use this c function yourself, make sure you have the alsa development packages installed. For AlmaLinux, this means the alsa-lib-devel package.
There are a few more that I have that I cant quite remember, alsa-utils is definitely one of them. Oh, well!

For use, make sure you are using the correct audio device.
Use "arecord -l" to get a list of devices.
What you are looking for is the number of the soundcard, and the number of the device on that card.
For example, I use Sound Card 0, Device 6 for my microphone.
You need to identify these two numbers and change the values in the config file.Currently, this only supports mono recording at 44100Hz.

Compilation:
perform the following command in your terminal:
"bash ./compile.sh -c"

Running:
run the program with the following command in your terminal:
./recorder 
