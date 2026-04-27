#include "AudioStream.hpp"
#include "Analyzer.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <complex>

int est_rms(int16_t *buffer, int len)
{
	int sum = 0;	
	for(int i = 0; i < len; i++)
	{
		sum += buffer[i] * buffer[i];
	}
	int mean = sum / len;

	return (int)sqrt(mean);
}

void printFFT(const std::vector<std::complex<float>>& data)
{
    int n = data.size();
    int numBars = 20;
    int binsPerBar = (n / 2) / numBars;

    std::cout << "\033[2J\033[H";

    for (int i = 0; i < numBars; i++)
    {
        float sum = 0.0f;

        for (int j = 0; j < binsPerBar; j++)
        {
            int idx = i * binsPerBar + j;
            sum += std::abs(data[idx]);
        }

        float avg = sum / binsPerBar;

        // log scaling (important for audio)
        float scaled = std::log10(1.0f + avg);

        int height = (int)(scaled * 20); // adjust 20 for taller bars

        std::cout << "|";
        for (int k = 0; k < height; k++)
            std::cout << "#";
        std::cout << " " << "\n";
    }

    std::cout << std::flush;
}

int main()
{
    std::string soundcard = "0";
    std::string devicenum = "6";
    std::string audiodev = "plughw:"+soundcard+","+devicenum;
    
    AudioStream mic(audiodev);
    
    int frame = mic.get_frame();

    std::vector<int16_t> buffer(frame);

    Analyzer analyzer;

    while(true)
    {
        int rc = mic.read(buffer.data());

        if(rc > 0)
        {
            //process here
	    
            /*
            int max = 0;

	        int rms = est_rms(buffer.data());
            max = rms;
	        int bar_length = (max * 50) / 32768;
            bar_length = std::min(bar_length, 50);
            std::string bar = "[" + std::string(bar_length, '#') + "]";
            */

            //get complex buffer of frame:
            //really, we should load a complex buffer while we capture the sound, to save on time.
            std::vector<std::complex<float>> cbuffer(frame);
            
            for(int i = 0; i < frame; i++)
            {
                float w = 0.5f * (1 - cos(2 * PI * i / (frame - 1)));
                cbuffer[i] = { (float)buffer[i] * w, 0.0f };
            }

            //FFT the vector:
            std::vector<std::complex<float>> output = analyzer.LiamFFT(cbuffer);

            printFFT(output);
        }
    }
}
