#include "pix5core.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>


IOSourceUnix::IOSourceUnix(const Str& base_path)
{
	_base = base_path;

	if(_base.len()==0)
		_base = "./";

	if(_base[_base.len()-1]!='/')
		_base += "/";
}

IOSourceUnix::~IOSourceUnix()
{
}

void IOSourceUnix::on_mount(const Str& mount_point)
{
	// Validate the base path at this point
}

File *IOSourceUnix::on_open(const FilePath& path, FileAccess::Type access)
{
	// Try to open the file
	Str fn = (_base+path.path().sub(1)).ptr();

	// Build the flags
	int flag = 0;

	switch(access)
	{
		case FileAccess::Read:			flag = O_RDONLY;					break;
		case FileAccess::Write:			flag = O_RDWR;						break;
		case FileAccess::Create:		flag = O_RDWR|O_CREAT;				break;
		case FileAccess::Truncate:		flag = O_RDWR|O_CREAT|O_TRUNC;		break;
	}

	int f = open(fn, flag, 0666);
	if(f==-1)
		E::OpenFile("Error opening file: %s: %s", path.full().ptr(), strerror(errno));

	// Success, wrap it in a FileUnix and return it
	// No need to track
	return new FileUnix(path, access==FileAccess::Read ? FileAccess::Read : FileAccess::Write, f);
}

void IOSourceUnix::on_get_files(const FilePath& path, List<FileEntry*> &files)
{
	// Try to open the path
	Str base = _base+path.path().sub(1)+"/";
	DIR *dir = opendir(base);

	if(!dir)
		E::DirIO("Error retrieving contents of %s: %s", path.full().ptr(), strerror(errno));

	// Read the files
	while(dirent *de = readdir(dir))
	{
		// Skip '.' and '..'
		if(strcmp(de->d_name, ".")==0 || strcmp(de->d_name, "..")==0)
			continue;

		// Get file info
		struct stat s;
		stat(base+de->d_name, &s);

		// Add a new entry
		files.add(new FileEntry(Str::build("%s:%s%s", path.mount_point().ptr(), path.dir().ptr(), de->d_name), S_ISDIR(s.st_mode) ? true : false, s.st_size));
	}

	closedir(dir);
}


