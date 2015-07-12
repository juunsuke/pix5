#include "pix5.h"
#include <SDL.h>

namespace PIX {
namespace Audio {


typedef struct Track
{
	SoundClip *clip;
	// The clip currently playing on that track, or NULL if none

	float pos;
	// Current playback position

	bool loop;
	// Wether the track loops

	bool playing;
	// Wether thr track is playing

	float adv_rate;
	// Position advance rate
} Track;


static SDL_AudioDeviceID _id = 0;
// Open device ID

static SDL_AudioSpec _spec;
// Audio specs for open device

static Track _tracks[MAX_TRACKS];
static int _num_track = 16;
// Sound tracks


void init()
{
	// Start the audio system with default values
	_id = 0;
	_num_track = 16;
	memset(_tracks, 0, sizeof(_tracks));
	start();
}

void done()
{
	stop();
}

void audio_cb(void *ud, uint8_t *ptr, int len)
{
	// Clear the buffer
	memset(ptr, 0, len);

	// Number of samples to produce
	int num = len/(2*_spec.channels);

	int16_t *out = (int16_t*)ptr;

	// Do each sample
	for(int c = 0; c<num; c++)
	{
		float tot[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

		// Check all the tracks
		for(int d = 0; d<_num_track; d++)
		{
			Track *t = _tracks+d;
			SoundClip *sc = t->clip;

			// Skip tracks that aren't playing
			if(!sc || !t->playing)
				continue;

			// Calc the current pos
			int pos = (int)t->pos;
			if(pos>=sc->_samples)
			{
				// Reached the end
				// Loop ?
				if(t->loop)
				{
					// Yes
					t->pos = 0;
					pos = 0;
				}
				else
				{
					// No, stop handling this track
					t->clip = NULL;
					break;
				}
			}

			// Add all the channel values to the totals
			int16_t *src = sc->_data + pos*sc->_num_chan;

			for(int e = 0; e<sc->_num_chan && e<_spec.channels; e++)
			{
				float v = (float)src[e];
				tot[e] += v*0.5f;
			}

			// Advance the source sample
			t->pos += t->adv_rate;
		}

		// Write the output data
		for(int e = 0; e<_spec.channels; e++)
			*(out++) = (int16_t)tot[e];
	}
}

void start(int freq, int chan, int buf_size)
{
	// Check for double-starts
	if(_id)
		E::AudioStarted("Audio::start(): Already started");

	// Try to open the device
	SDL_AudioSpec want;
	memset(&want, 0, sizeof(SDL_AudioSpec));

	want.freq = freq;
	want.format = AUDIO_S16;
	want.channels = chan;
	want.samples = buf_size;
	want.callback = audio_cb;

	Log::log("Opening audio device: %i Hz, %i channels, %i samples buffer", freq, chan, buf_size);

	_id = SDL_OpenAudioDevice(NULL, 0, &want, &_spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE|SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
	if(!_id)
		E::AudioStart("Error opening audio device: %s", SDL_GetError());

	Log::log("Success: %i Hz, %i channels.  Format: %04X", _spec.freq, _spec.channels, _spec.format);

	// Start playback
	SDL_PauseAudioDevice(_id, 0);
}

void stop()
{
	// Don't choke if not started
	if(!_id)
		return;

	// Close the audio device
	SDL_CloseAudioDevice(_id);
	_id = 0;
}

void set_tracks(int num)
{
	ASSERT(num>0 && num<=MAX_TRACKS, "Audio::set_tracks(): Invalid track number")
}

int get_tracks()
{
	return _num_track;
}

int get_free_track()
{
	// Find a free track
	for(int c = 0; c<_num_track; c++)
		if(!_tracks[c].clip)
			return c;

	// None found
	return -1;
}

int play(SoundClip *sc, bool loop, bool start, int track)
{
	// Play a sound clip

	// Look for a free track ?
	if(track==-1)
	{
		track = get_free_track();

		if(track==-1)
			// No free track
			return -1;
	}
	else
	{
		// Validate
		if(track>=_num_track)
			E::BadTrack("Audio::play(): Invalid track index");

		// Stop the track
		stop(track);
	}

	Log::debug("Using track %i", track);
		
	// Setup the track
	Track *t = _tracks+track;

	t->pos = 0;
	t->loop = loop;
	t->playing = start;
	t->adv_rate = (float)sc->freq()/(float)_spec.freq;

	t->clip = sc;

	return track;
}

void stop(int track)
{
	// Validate
	if(track<0 || track>=_num_track)
		E::BadTrack("Audio::stop(): Invalid track index");
}

void pause(int track)
{
	// Validate
	if(track<0 || track>=_num_track)
		E::BadTrack("Audio::pause(): Invalid track index");
}

void resume(int track)
{
	// Validate
	if(track<0 || track>=_num_track)
		E::BadTrack("Audio::resume(): Invalid track index");
}


}}

