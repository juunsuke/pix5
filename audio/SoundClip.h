#pragma once

class SoundClip
{
	SoundClip();
	// Used internally, use the static functions to load a clip

public:

	int _freq;
	// Frequency of the clip, in hertz

	int _chan;
	// Number of channels

	int _samples;
	// Number of samples

	Buffer<int16_t> _buf;
	// Raw sound data

	Str _cat;
	// Sound category

	float _vol;
	// Sound-specific volume




	~SoundClip();

	static SoundClip *load(const Str& fname, const Str& cat = "default");
	// Load an Ogg Vorbis (.ogg) file
	// 'cat' is the sound category the clip should be part of


	Str category() const { return _cat; }
	void set_category(const Str& cat) { _cat = cat; }
	// Get/set the sound category
	// Changing the category of a sound that is currently playing will
	// not have any effect until it is played again
};

