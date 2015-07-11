#pragma once

class SoundClip;

namespace Audio { void audio_cb(void*, uint8_t*, int); }

class SoundClip
{
	int _freq;
	// Frequency of the clip, in hertz

	int _num_chan;
	// Number of channels

	int _samples;
	// Number of samples

	uint16_t *_data;
	// Raw sound data


	SoundClip();
	// Used internally, use the static functions to load a clip

	friend void Audio::audio_cb(void*, uint8_t*, int);
	// The audio playing callback needs access to private sound clip data

public:

	~SoundClip();

	static SoundClip *load(const Str& fname);
	// Load an Ogg Vorbis (.ogg) file
};

