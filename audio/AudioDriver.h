#pragma once


class AudioDriver
{
protected:

public:

	AudioDriver()
	{
	}

	virtual ~AudioDriver()
	{
	}


	virtual void start(int freq, bool stereo) = 0;
	virtual void stop() = 0;
	// Start/stop the audio driver

	virtual void lock() = 0;
	virtual void unlock() = 0;
	// Lock/unlock the driver
	// While locked, no callback to Audio in another thread should happen

	virtual int get_freq() = 0;
	// Get the current frequency
};

