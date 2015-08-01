#include "pix5core.h"

namespace Cache
{


class CacheObject
{
public:

	Str name;
	// Cached object name

	void *ptr;
	// Opaque data

	void (*kill)(const Str&, void*);
	// Kill function


	CacheObject()
	{
		ptr = NULL;
		kill = NULL;
	}

	~CacheObject()
	{
		// Kill
		if(kill)
			kill(name, ptr);
	}


	int comp(CacheObject *o)
	{
		return name.comp(o->name);
	}
};



static SortedList<CacheObject*> _cache;
// All the cached items



void init()
{
}

void done()
{
	// Clear the cache
	clear();
}


void add(const Str& name, void *ptr, void (*kill)(const Str&, void*))
{
	ASSERT(!get(name), "Cache::add(): Item already in the cache")
	ASSERT(ptr, "Cache::add(): Invalid NULL data")

	// Create a new cache object
	CacheObject *co = new CacheObject();
	co->name = name;
	co->ptr = ptr;
	co->kill = kill;

	// Add it
	_cache.add(co);
}

static int find(const Str& name)
{
	// Find the cache object
	CacheObject co;
	co.name = name;

	return _cache.find(&co);
}

void *get(const Str& name)
{
	// Get a cached object
	int i = find(name);
	if(i<0)
		return NULL;

	return _cache[i]->ptr;
}

void remove(const Str& name)
{
	// Remove a cached object
	int i = find(name);
	if(i>=0)
		_cache.remove_del(i);
}

void clear()
{
	// Clear the cache
	_cache.clear_del();
}






}
