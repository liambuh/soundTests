#include "AudioStream.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

int est_rms(int16_t *buffer)
{
	int len = sizeof(buffer) / sizeof(buffer[0]);
	int sum = 0;	
	for(int i = 0; i < len; i++)
	{
		sum += buffer[i] * buffer[i];
	}
	int mean = sum / len;

	return (int)sqrt(mean);
}

int main()
{
    std::string soundcard = "0";
    std::string devicenum = "6";
    std::string audiodev = "plughw:"+soundcard+","+devicenum;
    
    AudioStream mic(audiodev);
    
    int frame = mic.get_frame();

    std::vector<int16_t> buffer(frame);

    while(true)
    {
        int rc = mic.read(buffer.data());

        if(rc > 0)
        {
            //process here
	    
            int max = 0;
	    /*
            for(int i = 0; i < rc; i++)
            {
                int val = buffer.data()[i];
                if(val < 0) val = -val;
                if(val > max) max = val;
            }
	    */
	    int rms = est_rms(buffer.data());
            max = rms;
	    int bar_length = (max * 50) / 32768;
            std::string bar = "[" + std::string(bar_length, '#') + "]";

            std::cout << bar << "\n";
        }
    }
}
