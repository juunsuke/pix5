#include "pix5.h"

namespace PIX {
namespace Cache {


class CachedTexture: public Obj
{
	PIXOBJ("CachedTexture")

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

	int comp(CachedTexture *o)
	{
		return name.comp(o->name);
	}
};

class CachedFont: public Obj
{
	PIXOBJ("CachedFont")

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



static SortedList<CachedTexture*> _tex;
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


static int find_tex_index(const Str& name)
{
	CachedTexture ct(name, NULL);
	return _tex.find(&ct);
}

Texture *texture(const Str& fname, bool try_load)
{
	// Check if its already loaded
	int i = find_tex_index(fname);
	if(i>=0)
		return _tex[i]->tex;

	if(!try_load)
		return NULL;

	// Load it
	Texture *tex = Texture::load(fname);

	// Add it to the cache
	_tex.add(new CachedTexture(fname, tex));

	return tex;
}

void texture(const Str& name, Texture *tex)
{
	// Make sure it doesn't exist
	if(has_texture(name))
		E::DoubleCache("Cache::texture(): Manually added texture '%s' already exists", name.ptr());

	// Add it
	_tex.add(new CachedTexture(name, tex));
}

bool has_texture(const Str& name)
{
	// Check if we have this texture
	return find_tex_index(name)>=0;
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
