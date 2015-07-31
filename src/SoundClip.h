#pragma once

class SoundClip;

namespace Audio { void audio_cb(void*, uint8_t*, int); }

class SoundClip: public Obj
{
	PIXOBJ("SoundClip")

	int _freq;
	// Frequency of the clip, in hertz

	int _num_chan;
	// Number of channels

	int _samples;
	// Number of samples

	float *_data;
	// Raw sound data
	// Kept as float for easier math until final output

	Str _cat;
	// Sound category


	SoundClip();
	// Used internally, use the static functions to load a clip

	friend void Audio::audio_cb(void*, uint8_t*, int);
	// The audio playing callback needs access to private sound clip data

public:

	~SoundClip();

	static SoundClip *load(const Str& fname, const Str& cat = "default");
	// Load an Ogg Vorbis (.ogg) file
	// 'cat' is the sound category the clip should be part of


	inline int freq() const { return _freq; }
	inline int num_chan() const { return _num_chan; }
	inline int samples() const { return _samples; }
	// Get info about the clip

	Str category() const { return _cat; }
	void set_category(const Str& cat) { _cat = cat; }
	// Get/set the sound category
	// Changing the category of a sound that is currently playing will
	// not have any effect until it is played again
};

