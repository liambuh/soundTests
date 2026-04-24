

/*

This example reads from the default PCM device
and writes to standard output for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>

struct AudioHandle {
  snd_pcm_t *handle;
  int channels;
  snd_pcm_uframes_t frames;
};

AudioHandle* audio_open(const char *device, int channels, int rate)
{
  AudioHandle* h = malloc(sizeof(AudioHandle));

  snd_pcm_open(&h->handle, device, SND_PCM_STREAM_CAPTURE, 0);

  snd_pcm_hw_params_t *params;

  snd_pcm_hw_params_alloca(&params); // Allocate Hardware Params Object
  snd_pcm_hw_params_any(h->handle, params); // Fill params with default values

  /* Set the desired hardware parameters. */
  snd_pcm_hw_params_set_access(h->handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); // Interleaved Mode
  snd_pcm_hw_params_set_format(h->handle, params, SND_PCM_FORMAT_S16_LE); // Signed, 16-bit, little-endian format.
  snd_pcm_hw_params_set_channels(h->handle, params, channels); // Mono (2 for stereo)

  unsigned int r = rate;
  snd_pcm_hw_params_set_rate_near(handle, params, &r, 0); //set sample rate (should be 44100)

  snd_pcm_uframes_t frames = 1024;
  snd_pcm_hw_params_set_period_size_near(h->handle, params, &frames, 0); //set period size
  snd_pcm_hw_params_get_period_size(params, &frames, 0); //retrieve set period size (can be different from what we sent in)

  snd_pcm_hw_params(h->handle, params); //set parameters

  h->channels = channels;
  h->frames = frames;

  return h;
}

int audio_get_frame(AudioHandle *audio)
{
  return audio->frame;
}

int audio_read(AudioHandle *audio, int16_t *buffer)
{
  return snd_pcm_readi(audio->handle, buffer, audio->frames);
}

void audio_close(AudioHandle *audio)
{
  snd_pcm_drain(audio->handle);
  snd_pcm_close(audio->handle);
  free(audio);
}