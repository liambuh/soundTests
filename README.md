Hello!

I am messing around with audio streaming and processing, and have made very minimal adjustments to a wonderful recording program found at the following link: https://www.linuxjournal.com/article/6735

This repo has 2 main folders/sub-projects in it: 'standalone', and 'cpp'.

We will go over both below, but a quick guide for the similar build scripts in both:

To compile either project, run the following command while in its respective directory:
'bash ./build.sh -c'
To run either compiled binary, do a similar command:
'bash ./build.sh -r'
You can build and compile with the -cr or -rc options.

Note that while the build.sh scripts are similar, they execute different commands.

== STANDALONE ==

Setup/Prerequisits:
To develop and use this c program yourself (found in the simple_recording.c file), make sure you have the alsa development packages installed. For AlmaLinux, this means the alsa-lib-devel package.
There are a few more that I have that I cant quite remember, alsa-utils is definitely one of them. Oh, well!

For use, make sure you are using the correct audio device.
Use "arecord -l" to get a list of devices.
What you are looking for is the number of the soundcard, and the number of the device on that card.
For example, I use Sound Card 0, Device 6 for my microphone.
You need to identify these two numbers and change the values in the config.txt file.
Currently, this only supports mono recording at 44100Hz.

== cpp ==

The cpp program is a proof-of-concept for building the recorder (now in an audio.c file) as an object, and then passing that through to a cpp program for procesing.

Press Ctrl+C to quit the program at any time while running.

Note that if your recording device is not on card 0, device 6, this program will NOT work for you as-written, and you will need to go into the main.cpp file and change the values for the sound device you are using. Follow the steps detailed in the STANDALONE section for finding your input device's identifying numbers.

Another note: you can just call ./app in the shell to quickly run the compiled program, no need to call the build script really unless you are building the program. 
