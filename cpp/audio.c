

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

int main() {
  long loops;
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;

  //configure the device from the config file.
  int card = 0;
  int device = 0;
  read_config(&card, &device);
  printf("card = %d, dev = %d\n",card,device);

  char device_name[32];
  snprintf(device_name, sizeof(device_name), "plughw:%d,%d", card, device);
  printf("deivce name: %s\n",device_name);
  
  //return 0;

  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, device_name,
                    SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 1);

  /* 44100 bits/second sampling rate (CD quality) */
  val = 44100;
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);

  /* Set period size to N frames. */
  frames = 1024;
  snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params,
                                      &frames, &dir);
  size = frames * 2; /* 2 bytes/sample, 1 channels */
  buffer = (char *) malloc(size);

  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params,
                                         &val, &dir);
  loops = 5000000 / val;

  while (loops > 0) {
    loops--;
    rc = snd_pcm_readi(handle, buffer, frames);
    if (rc == -EPIPE) {
      /* EPIPE means overrun */
      fprintf(stderr, "overrun occurred\n");
      snd_pcm_prepare(handle);
    } else if (rc < 0) {
      fprintf(stderr,
              "error from read: %s\n",
              snd_strerror(rc));
    } else if (rc != (int)frames) {
      fprintf(stderr, "short read, read %d frames\n", rc);
    }

    int16_t *samples = (int16_t *)buffer;
    int sample_count = frames; //mono

    int max = 0;
    for(int i = 0; i < sample_count; i++)
    {
	int val = samples[i];
	if(val < 0) val = -val;
	if(val > max) max = val;
    }

    int bar_length = (max * 50) / 32768;
    printf("\r[");
    for(int i = 0; i < 50; i++)
    {
	if (i < bar_length) printf("#");
	else printf(" ");
    }
    printf("]");
    //printf("max=%d\n",max);
    fflush(stdout);

    //rc = write(1, buffer, size);
    if (rc != size)
      fprintf(stderr,
              "short write: wrote %d bytes\n", rc);
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);

  return 0;
}

