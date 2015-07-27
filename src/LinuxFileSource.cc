#include "pix5.h"
#include <errno.h>

namespace PIX {


LinuxFileSource::LinuxFileSource(const Str& path)
{
	// Validate the path
	if(path.len()==0)
		E::BadPath("LinuxFileSource::LinuxFileSource(): Empty path");

	_path = path;

	// Append a slash at the end if needed
	if(_path[_path.len()-1]!='/')
		_path += "/";

	// Try to open "." inside the base path
	FILE *f = fopen(_path+".", "rb");
	if(!f)
		E::BadPath("LinuxFileSource::LinuxFileSource(): Invalid path: '%s'", path.ptr());
}

LinuxFileSource::~LinuxFileSource()
{
}

Str LinuxFileSource::get_info_str()
{
	return Str::build("Linux file system (%s)", _path.ptr());
}

File *LinuxFileSource::open_file(const FilePath& fname, FileAccess::Type access)
{
	// Try to open the file with the specified access
	Str fn = _path + fname.path();
	FILE *f = NULL;

	switch(access)
	{
		case FileAccess::ReadOnly:
			// Read only
			f = fopen(fn, "rb");
			break;

		case FileAccess::ReadWrite:
		case FileAccess::Append:
			// Read/write (will reposition later if access is Append)
			f = fopen(fn, "r+b");
			break;

		case FileAccess::Create:
			// Open as read/write, and create if it doesn't exist
			// First try to open as read/write
			f = fopen(fn, "r+b");
			if(f)
				break;

			// Not working, try to create
			// FALL THROUGH
		case FileAccess::Purge:
			// Open as read/write, create if it doesn't exist, and purge contents if it does
			f = fopen(fn, "w+b");
			break;
	}

	if(!f)
		// Failure to open the file
		E::OpenFile("Error opening '%s': %s", fn.ptr(), strerror(errno));

	// Success
	// Create a CFile for it
	CFile *cf = new CFile(this, access, f);

	return cf;
}



}
