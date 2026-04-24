#pragma once
#include <string>
#include <cstdint>

extern "C" {
#include "audio.h"
}

class AudioStream
{
    public:
        AudioStream(const std::string& device, int channels = 1, int rate = 44100)
        {
            handle = audio_open(device.c_str(), channels, rate);
        }

        int read(int_16t* buffer)
        {
            return audio_read(handle, buffer)
        }

        int get_frame()
        {
            return audio_get_frame(handle);
        }

        ~AudioStream()
        {
            if(handle) audio_close(handle);
        }
    private:
        AudioHandle* handle = nullptr;
}
