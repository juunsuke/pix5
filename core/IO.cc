#include "pix5core.h"

namespace IO {


class Mount
{
public:

	Str mount_point;
	IOSource *src;

	Mount(const Str& mount_point, IOSource *src)
	{
		this->mount_point = mount_point;
		this->src = src;
	}

	~Mount()
	{
	}
};


static List<Mount*> _mounts;
// All the mounts




void init()
{
	// Mount the basic sources
	mount("home", new IOSourceUnix("."));
	mount("root", new IOSourceUnix("/"));
}

void done()
{
	// Forcibly unmount everything
	while(_mounts.size())
		unmount(_mounts[_mounts.size()-1]->mount_point, true);
}


static int find_mount(const Str& mount_point)
{
	// Find a mount by its mount point
	for(int c = 0; c<_mounts.size(); c++)
		if(_mounts[c]->mount_point==mount_point)
			return c;

	// Not found
	return -1;
}

static void validate_mount_point(const Str& mount_point)
{
	// Empty mount points are invalid
	if(mount_point.len()==0)
		E::BadMountPoint("IO::mount(): Empty mount point");

	// Only allow alphanumeric characters
	for(int c = 0; c<mount_point.len(); c++)
	{
		char ch = mount_point[c];

		if(ch>='A' && ch<='Z')
			continue;
		if(ch>='a' && ch<='z')
			continue;
		if(ch=='_')
			continue;
		if(c && (ch>='0' && ch<='9'))
			continue;

		// Invalid character
		E::BadMountPoint("IO::mount(): Invalid character in mount point: '%s'", mount_point.ptr());
	}

	// Look for duplicates
	if(find_mount(mount_point)!=-1)
		E::BadMountPoint("IO::mount(): Mount point already in use: '%s'", mount_point.ptr());

	// Seems valid
}

void mount(const Str& mount_point, IOSource *src)
{
	// Validate the mount point
	validate_mount_point(mount_point);

	// Let the source know it's about to be mounted
	src->on_mount(mount_point);

	// Mount it
	_mounts.add(new Mount(mount_point, src));

	Log::log("'%s' mounted on '%s:'", src->get_desc().ptr(), mount_point.ptr());
}

void unmount(const Str& mount_point, bool force)
{
	// Find the mount
	int i = find_mount(mount_point);
	if(i==-1)
		E::BadMountPoint("IO::unmount(): Mount point not found: '%s'", mount_point.ptr());

	Mount *mnt = _mounts[i];

	try
	{
		// Let the source know of the unmount
		mnt->src->on_unmount(mount_point, force);
	}
	catch(Error e)
	{
		// If this isn't a forced unmount, relay the error
		if(!force)
			throw;
	}

	// Delete the source and remove the mount
	IOSource *src = mnt->src;
	_mounts.remove_del(i);
	delete src;
}

File *open(const FilePath& path, FileAccess::Type access)
{
	// Validate the path
	if(!path.valid())
		E::OpenFile("IO::open(): Invalid path: %s", path.error().ptr());

	// Find the mount point
	int i = find_mount(path.mount_point());
	if(i==-1)
		E::OpenFile("IO::open(): Invalid mount-point: %s", path.full().ptr());

	#ifdef DBG
	Log::debug("Opening file: %s", path.full().ptr());
	#endif

	// Ask the source to open the file
	return _mounts[i]->src->on_open(path, access);
}

void *read_file(const FilePath& path, int *size)
{
	// Try to open the file
	File *f = open(path);

	// Get its size and allocate the buffer
	int sz = f->size();
	char *buf = (char*)malloc(sz+1);

	// Read the file
	try
	{
		sz = f->read(buf, sz);
		delete f;
	}
	catch(Error)
	{
		// Some failure
		free(buf);
		delete f;
		throw;
	}

	buf[sz] = 0;

	if(size)
		*size = sz;

	return buf;
}

void get_files(const FilePath& path, List<FileEntry*> &files)
{
	if(!path.valid())
		E::DirIO("IO::get_files(): Invalid path");

	// Find the mount point
	int i = find_mount(path.mount_point());
	if(i==-1)
		E::OpenFile("IO::get_files(): Invalid mount-point: %s", path.full().ptr());

	#ifdef DBG
	Log::debug("Retrieving directory content: %s", path.full().ptr());
	#endif
	
	// Let the source do the rest
	_mounts[i]->src->on_get_files(path, files);
}

void get_mounts(List<MountEntry*> &mounts)
{
	// Add all the mounts
	for(int c = 0; c<_mounts.size(); c++)
		mounts.add(new MountEntry(_mounts[c]->mount_point, _mounts[c]->src->get_desc()));
}


}

