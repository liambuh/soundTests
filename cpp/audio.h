#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AudioHandle AudioHandle;

AudioHandle* audio_open(const char *device, int channels, int rate);
int audio_read(AudioHandle *audio, int16_t *buffer);
void audio_close(AudioHandle *audio);

#ifdef __cplusplus
}
#endif