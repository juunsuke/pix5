#include "pix5audio.h"


namespace Audio
{


class Track
{
public:

	SoundClip *_sc;
	// The soundclip playing on the track

	float _vol;
	float _pan;
	float _pitch;
	// Track parameters

	double _pos;
	// Current position, between 0 and 1

	double _adv;
	// Advance value, how much _pos increases for every sample

	bool _repeat;
	// Wether the track repeats

	bool _active;
	// Wether the track is active

	Track *_next, *_prev;
	// Links for the doubled linked list of active tracks


	Track()
	{
		_active = false;
	}

	~Track()
	{
	}
};



static AudioDriver *_driver = NULL;
// The current audio driver

static List<Track*> _tracks;
// All the allocated tracks

static Track *_first, *_last;
// Doubly linked list of active tracks




void init()
{
	// Allocate a default amount number of tracks
	for(int c = 0; c<64; c++)
		_tracks.add(new Track());

	// Prepare the linked list of active tracks
	_first = new Track();
	_last = new Track();

	_first->_next = _last;
	_first->_prev = NULL;

	_last->_next = NULL;
	_last->_prev = _first;

	// Select a driver
	_driver = new SDLAudio();

	// Start the driver with default settings
	_driver->start(44100, true);
}

void done()
{
	// Stop and elete the audio driver
	if(_driver)
	{
		_driver->stop();
		delete _driver;
		_driver = NULL;
	}

	_tracks.clear_del();

	if(_first)
		delete _first;
	if(_last)
		delete _last;

	_first = NULL;
	_last = NULL;
}

static void unlink_track(Track *trk)
{
	// Unlink the track
	trk->_prev->_next = trk->_next;
	trk->_next->_prev = trk->_prev;

	trk->_active = false;
}

void set_tracks(int num)
{
	if(num==_tracks.size())
		return;

	// We need to lock while doing this
	_driver->lock();

	if(num>_tracks.size())
	{
		// Increasing the number of tracks is very simple
		for(int c = _tracks.size(); c<num; c++)
			_tracks.add(new Track());
	}
	else
	{
		// When reducing the number of tracks, care must be taken to
		// remove active tracks
		while(_tracks.size()>num)
		{
			// Unlink it if it's linked
			Track *trk = _tracks[_tracks.size()-1];

			if(trk->_active)
				unlink_track(trk);

			_tracks.remove_del(_tracks.size()-1);
		}
	}

	_driver->unlock();
}

static Track *find_free_track(int& i)
{
	// Look for a free track
	for(int c = 0; c<_tracks.size(); c++)
	{
		Track *trk = _tracks[c];

		if(!trk->_active)
		{
			i = c;
			return trk;
		}
	}

	// No free track
	return NULL;
}

int play(SoundClip *sc, int track, bool repeat, float vol, float pan, float pitch, float pos)
{
	Track *trk = NULL;

	if(track==-1)
	{
		// Find free track
		_driver->lock();

		trk = find_free_track(track);

		_driver->unlock();

		// Abort if there were no free tracks
		if(!trk)
			return -1;
	}
	else
	{
		if(track<0 || track>=_tracks.size())
			E::Assert("Audio::play(): Invalid track index");

		// Unlink it if it's active
		_driver->lock();

		trk = _tracks[track];
		if(trk->_active)
			unlink_track(trk);

		_driver->unlock();
	}

	// Setup the track
	trk->_sc = sc;
	trk->_repeat = repeat;
	trk->_vol = vol;
	trk->_pan = Math::fclamp(pan);
	trk->_pitch = pitch;
	trk->_pos = (double)Math::fclamp(pos);
	trk->_adv = (1.0 / (double)sc->_samples) * ((double)sc->_freq / (double)_driver->get_freq());
	trk->_active = true;

	// Link the track in the active list
	_driver->lock();

	trk->_prev = _first;
	trk->_next = _first->_next;

	trk->_prev->_next = trk;
	trk->_next->_prev = trk;

	_driver->unlock();

	return track;
}

void mix(int16_t *dest, int num, bool stereo)
{
	// Skip if there are no playing tracks
	if(_first->_next==_last)
	{
		memset(dest, 0, num*2*(stereo?2:1));
		return;
	}

	// Mix every sample
	for(int c = 0; c<num; c++)
	{
		float v1 = 0;
		float v2 = 0;

		// Go through each track
		for(Track *trk = _first->_next; trk!=_last; trk = trk->_next)
		{
			// Get the final volume for this track
			float vol = trk->_vol * trk->_sc->_vol;

			// Get a sample
			int pos = (int)(trk->_pos*(double)trk->_sc->_samples);
			if(pos>=trk->_sc->_samples)
			{
				// Gone beyond the end
				// Loop or unlink
				if(trk->_repeat)
					trk->_pos = 0;
				else
					unlink_track(trk);

				// Either way don't process it at this time
				continue;
			}

			if(trk->_sc->_chan==2)
				pos *= 2;

			// Get the sample
			float smp1 = (float)trk->_sc->_buf[pos];
			v1 += vol*smp1;

			if(stereo)
			{
				float smp2 = trk->_sc->_chan==2 ? (float)trk->_sc->_buf[pos+1] : smp1;
				v2 += vol*smp2;
			}

			// Advance the track
			trk->_pos += trk->_adv;
		}

		// Set the value
		*(dest++) = (int16_t)v1;

		if(stereo)
			*(dest++) = (int16_t)v2;
	}
}

void stop_clip(SoundClip *sc)
{
	if(!_driver)
		return;

	// Stop a clip on all tracks playing it
	_driver->lock();

	// Check all active tracks
	for(Track *trk = _first; trk->_next!=_last;)
		if(trk->_next->_sc==sc)
			unlink_track(trk->_next);
		else
			trk = trk->_next;

	_driver->unlock();
}





}
