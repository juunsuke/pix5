#include "pix5audio.h"
#include "vorbis/vorbisfile.h"



SoundClip::SoundClip()
{
}

SoundClip::~SoundClip()
{
	// Make sure no track is playing this clip
	Audio::stop_clip(this);
}

SoundClip *SoundClip::load(const Str& fname, const Str& cat)
{
	// Try to open the file
	OggVorbis_File vf;

	int err = ov_fopen(fname, &vf);
	switch(err)
	{
		case 0:
			// Success
			break;

		case OV_ENOTVORBIS:
		case OV_EVERSION:
		case OV_EBADHEADER:
			E::LoadVorbis("Invalid Ogg Vorbis file: %s", fname.ptr());

		default:
			E::LoadVorbis("Unknown error loading Ogg Vorbis file: %s", fname.ptr());
	}

	try
	{
		Log::log("Loading Ogg Vorbis file: %s", fname.ptr());

		// Get info about the stream
		vorbis_info *vi = ov_info(&vf, -1);
		int samples = ov_pcm_total(&vf, -1);

		#ifdef DBG
		Log::debug("Bitstream is %i Hz, %i channels, %i samples", vi->rate, vi->channels, samples);
		#endif

		if(vi->channels>2)
			E::LoadVorbis("Only mono and stereo samples are supported");

		// Load the clip
		SoundClip *sc = new SoundClip();
		sc->_freq = vi->rate;
		sc->_chan = vi->channels;
		sc->_samples = samples;
		sc->_cat = cat;
		sc->_buf.resize(samples*sc->_chan);
		sc->_vol = 0.5f;
		
		// Load the file data
		int bs = 0;
		char *ptr = (char*)sc->_buf.ptr();

		try
		{
			// Try to read, 4k at a time because of reasons
			for(;;)
			{
				int ret = ov_read(&vf, ptr, 4096, 0, 2, 1, &bs);

				// Done ?
				if(!ret)
					break;

				if(ret<0)
					E::LoadVorbis("Error while loading sound samples");

				ptr += ret;
			}
		}
		catch(Error)
		{
			delete sc;
			throw;
		}
	
		// Done reading the data
		ov_clear(&vf);

		return sc;
	}
	catch(Error)
	{
		ov_clear(&vf);
		throw;
	}
}



