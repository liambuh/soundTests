#include "AudioStream.hpp"
#include <iostream>

int main()
{
    AudioStream mic("plughw:0,6");
    int frame = mic.get_frame();

    int16_t buffer[frame];

    while(true)
    {
        int rc = mic.read(buffer, frame);

        if(rc > 0)
        {
            //process here
            std::cout << buffer[0] << "\n";
        }
    }
}
