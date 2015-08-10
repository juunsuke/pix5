#include "pix5audio.h"
#include <SDL.h>


namespace Audio
{
	void mix(int16_t *dest, int num, bool stereo);
}



SDLAudio::SDLAudio()
{
	_id = 0;
}

SDLAudio::~SDLAudio()
{
}

static void sdl_audio_cb(void *udata, uint8_t *stream, int len)
{
	// Mix audio data
	SDLAudio *sa = (SDLAudio*)udata;
	Audio::mix((int16_t*)stream, (len/2)/sa->_chan, sa->_chan==2);
}


void SDLAudio::start(int freq, bool stereo)
{
	// Init SDL audio
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec want, have;

	memset(&want, 0, sizeof(want));
	want.freq = freq;
	want.format = AUDIO_S16;
	want.channels = stereo ? 2 : 1;
	want.samples = 4096;
	want.callback = sdl_audio_cb;
	want.userdata = this;

	// Open the device
	_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE|SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
	if(!_id)
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		E::SDL("Error opening audio device: %s", SDL_GetError());
	}

	_freq = have.freq;
	_chan = have.channels;

	Log::log("Opened SDL audio device: %i Hz %i channels", _freq, _chan);

	SDL_PauseAudioDevice(_id, 0);
}

void SDLAudio::stop()
{
	// Shutdown
	if(!_id)
		return;

	SDL_CloseAudioDevice(_id);
	_id = 0;

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SDLAudio::lock()
{
	SDL_LockAudioDevice(_id);
}

void SDLAudio::unlock()
{
	SDL_UnlockAudioDevice(_id);
}

int SDLAudio::get_freq()
{
	return _freq;
}


