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

class CachedFont
{
public:

	Str name;
	Font *fnt;
	int ptsize;

	CachedFont(const Str& name, Font *fnt, int ptsize)
	{
		this->name = name;
		this->fnt = fnt;
		this->ptsize = ptsize;
	}

	~CachedFont()
	{
		delete fnt;
	}
};



static List<CachedTexture*> _tex;
static List<CachedFont*> _fnt;
// All the cached items



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
	_fnt.clear_del();
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

Font *font(const Str& fname, int ptsize)
{
	// Check if its already loaded
	for(int c = 0; c<_fnt.size(); c++)
		if(_fnt[c]->name==fname && _fnt[c]->ptsize==ptsize)
			return _fnt[c]->fnt;

	// Load it
	Font *fnt = Font::load(fname, ptsize);

	// Add it to the cache
	_fnt.add(new CachedFont(fname, fnt, ptsize));

	return fnt;
}

Font *font(const uint8_t *data, uint32_t len, int ptsize)
{
	// Load a memory font
	// Build the name from the pointer
	Str name = Str::build("mem_%lli", (long long)(intptr_t)data);

	for(int c = 0; c<_fnt.size(); c++)
		if(_fnt[c]->name==name && _fnt[c]->ptsize==ptsize)
			return _fnt[c]->fnt;

	// Load it
	Font *fnt = Font::load(data, len, ptsize);

	// Add it to the cache
	_fnt.add(new CachedFont(name, fnt, ptsize));

	return fnt;
}




}}
