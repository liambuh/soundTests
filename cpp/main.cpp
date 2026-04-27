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

std::vector<float> buildLogBins(const std::vector<std::complex<float>>& data, int numBars)
{
    int n = data.size();
    int half = n / 2;

    std::vector<float> bars(numBars, 0.0f);
    float maxValue = 0.0f;
    for (int i = 0; i < numBars; i++)
    {
        float startFrac = std::pow((float)i / numBars, 2.0f);
        float endFrac   = std::pow((float)(i + 1) / numBars, 2.0f);

        int startBin = (int)(startFrac * half);
        int endBin   = (int)(endFrac * half);

        if (endBin <= startBin)
            endBin = startBin + 1;

        float sum = 0.0f;
        int count = 0;

        for (int j = startBin; j < endBin && j < half; j++)
        {
            sum += std::abs(data[j]);
            count++;
        }

        bars[i] = (count > 0) ? sum / count : 0.0f;

        bars[i] = log10(1.0f + bars[i]);

        if(bars[i] > maxValue)
            maxValue = bars[i];
    }

    //normalize:
    for(int i = 0; i < numBars; i++)
    {
        bars[i] /= maxValue;
    }

    return bars;
}

void drawSpectrum(const std::vector<float>& bars)
{
    int height = 20;

    std::cout << "\033[2J\033[H";

    for (int row = height; row >= 0; row--)
    {
        for (float v : bars)
        {
            if (v * height >= row)
                std::cout << "# ";
            else
                std::cout << "  ";
        }
        std::cout << "\n";
    }

    std::cout.flush();
}

void printTimeSignal(std::vector<int16_t>& buffer)
{
    int max = 0;

    int rms = est_rms(buffer.data(), buffer.size());
    max = rms;
    int bar_length = (max * 50) / 32768;
    bar_length = std::min(bar_length, 50);
    std::string bar = "[" + std::string(bar_length, '#') + "]";
}

void printFFT(const std::vector<std::complex<float>>& data)
{
    int n = data.size();
    int numBars = 20;
    int binsPerBar = (n / 2) / numBars;

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
            auto bars = buildLogBins(output, 50);

            std::cout << "\033[2J\033[H";
            printTimeSignal(buffer);
            drawSpectrum(bars);
            //printFFT(output);
        }
    }
}
