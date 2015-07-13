#include "pix5.h"

namespace PIX {
namespace Cache {


class CachedTexture
{
public:

	Str name;
	Texture *tex;

	CachedTexture(const Str& name, Texture *tex)
	{
		this->name = name;
		this->tex = tex;
	}

	~CachedTexture()
	{
		delete tex;
	}
};



static List<CachedTexture*> _tex;
// All the cached textures



void init()
{
}

void done()
{
	flush();
}


void flush()
{
	// Flush everything
	_tex.clear_del();
}


Texture *texture(const Str& fname)
{
	// Check if its already loaded
	for(int c = 0; c<_tex.size(); c++)
		if(_tex[c]->name==fname)
			return _tex[c]->tex;

	// Load it
	Texture *tex = Texture::load(fname);

	// Add it to the cache
	_tex.add(new CachedTexture(fname, tex));

	return tex;
}



}}
