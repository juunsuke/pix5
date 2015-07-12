#include "pix5.h"
#include "vorbis/vorbisfile.h"

namespace PIX {



SoundClip::SoundClip()
{
	_data = NULL;
}

SoundClip::~SoundClip()
{
	if(_data)
		free(_data);
}

SoundClip *SoundClip::load(const Str& fname)
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

		// Load the clip
		SoundClip *sc = new SoundClip();
		sc->_freq = vi->rate;
		sc->_num_chan = vi->channels;
		sc->_samples = samples;
		sc->_data = (float*)malloc(samples*sc->_num_chan*4);
		
		// Load the file data
		int pos = 0;
		int bs = 0;

		try
		{
			// Try to read
			for(;;)
			{
				int16_t buf[4096];
				int ret = ov_read(&vf, (char*)buf, 4096, 0, 2, 1, &bs);

				// Done ?
				if(!ret)
					break;

				if(ret<0)
					E::LoadVorbis("Error while loading sound samples");

				ret /= 2;
				for(int c = 0; c<ret; c++)
					sc->_data[pos++] = buf[c];
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



}

