#include "AudioStream.hpp"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    AudioStream mic("plughw:0,6");
    int frame = mic.get_frame();

    std::vector<int16_t> buffer(frame);

    while(true)
    {
        int rc = mic.read(buffer.data());

        if(rc > 0)
        {
            //process here
            int max = 0;
            for(int i = 0; i < rc; i++)
            {
                int val = samples[i];
                if(val < 0) val = -val;
                if(val > max) max = val;
            }

            int bar_length = (max * 50) / 32768;
            std::string bar = "[";
            for(int i = 1; i <= bar_length; i++)
            {
                bar = bar.append("#");
            }
            bar = bar.append("]");

            std::cout << bar << "\n";
        }
    }
}
