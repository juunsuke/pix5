#include "pix5.h"

namespace PIX {
namespace IO {


class Mount
{
public:

	Str mount_point;
	FileSource *fs;
};


static List<Mount*> _mounts;
// List of all the mounts



void init()
{
	// Auto-mount root and home
	mount("root", new LinuxFileSource("/"));
	mount("home", new LinuxFileSource("."));
}

void done()
{
	// Force unmount everything, starting from the end
	for(int c = _mounts.size()-1; c>=0; c--)
	{
		Mount *mnt = _mounts[c];

		// Force unmount
		try
		{
			mnt->fs->on_force_unmount(mnt->mount_point);
		}
		catch(Error)
		{
		}

		// Delete it
		delete mnt->fs;
		delete mnt;
	}

	_mounts.clear_nodel();
}

static Mount *get_mount(const Str& mount_point, int *i = NULL)
{
	// Check if the given mount point is mounted
	for(int c = 0; c<_mounts.size(); c++)
	{
		Mount *mnt = _mounts[c];

		if(mnt->mount_point==mount_point)
		{
			if(i)
				*i = c;

			return mnt;
		}
	}

	// Not found
	return NULL;
}

static void validate_mount_point(const Str& mount_point, const char *func)
{
	// Validate the mount point
	if(mount_point.len()==0)
		E::BadMountPoint("IO::%s(): Empty mount-point", func);

	// Validate the characters
	for(int c = 0; c<mount_point.len(); c++)
	{
		char ch = mount_point[c];

		// Alpha characters and underscores are valid
		if((ch>='a' && ch<='z') || ch=='_')
			continue;

		// Numeric characters are valid after the first character
		if(c && (ch>='0' && ch<='9'))
			continue;

		// Invalid character
		E::BadMountPoint("IO::%s(): Invalid character in mount point: '%c'", func, ch);
	}
}

void mount(const Str& mount_point, FileSource *fs)
{
	// Make sure the mount point is valid
	validate_mount_point(mount_point, "mount");

	// Make sure it's not already mounted
	if(get_mount(mount_point))
		E::BadMountPoint("IO::mount(): Mount-point '%s' is already in use", mount_point.ptr());

	// Mount it
	Mount *mnt = new Mount();
	mnt->mount_point = mount_point;
	mnt->fs = fs;

	_mounts.add(mnt);

	// Notify the file source
	try
	{
		fs->on_mount(mount_point);
	}
	catch(Error)
	{
		// Some sort of failure, force an unmount
		try
		{
			fs->on_force_unmount(mount_point);
		}
		catch(Error)
		{
		}

		delete mnt;
		throw;
	}

	Log::log("Mounted '%s' on '%s:'", fs->get_info_str().ptr(), mount_point.ptr());
}

void unmount(const Str& mount_point)
{
	// Find the mount
	int i;

	Mount *mnt = get_mount(mount_point, &i);
	if(!mnt)
		E::BadMountPoint("IO::unmount(): Nothing mounted at '%s'", mount_point.ptr());

	// Unmount it
	_mounts.remove_nodel(i);

	mnt->fs->on_unmount(mnt->mount_point);
	delete mnt->fs;
	delete mnt;
}

File *open(const FilePath& fname, FileAccess::Type access)
{
	if(!fname.valid())
		E::BadPath("IO::open(): Invalid file path: '%s'", fname.full().ptr());

	// Get the mount
	Mount *mnt = get_mount(fname.mount_point());
	if(!mnt)
		E::BadPath("IO::open(): Invald mount point for file path: '%s'", fname.full().ptr());

	// Let the file source do the opening
	return mnt->fs->open_file(fname, access);
}



}}
