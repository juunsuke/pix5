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

		// Load the file data
		int sz = samples*sc->_num_chan*2;
		uint8_t *buf = (uint8_t*)malloc(sz);
		int pos = 0;
		int bs = 0;

		try
		{
			// Try to read
			for(;;)
			{
				int ret = ov_read(&vf, (char*)buf+pos, sz-pos, 0, 2, 1, &bs);

				// Done ?
				if(!ret)
					break;

				if(ret<0)
					E::LoadVorbis("Error while loading sound samples");

				pos += ret;
			}
		}
		catch(Error)
		{
			free(buf);
			delete sc;
			throw;
		}
	
		// Done reading the data
		sc->_data = (uint16_t*)buf;
		
		ov_clear(&vf);

		#ifdef DBG
		Log::debug("Successfully decoded %i bytes of sound sample data", pos);
		#endif

		return sc;
	}
	catch(Error)
	{
		ov_clear(&vf);
		throw;
	}
}



}

