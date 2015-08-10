#pragma once


class SDLAudio: public AudioDriver
{
public:

	uint32_t _id;
	// SDL device ID, when open

	int _freq;
	// Current frequency

	int _chan;
	// Current number of channels


	void start(int freq, bool stereo);
	void stop();
	void lock();
	void unlock();
	int get_freq();


	SDLAudio();
	~SDLAudio();


	
};

