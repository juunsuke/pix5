#include "pix5core.h"
#include <unistd.h>
#include <errno.h>


FileUnix::FileUnix(const FilePath& path, FileAccess::Type access, int f)
{
	_path = path;
	_access = access;
	_f = f;
}

FileUnix::~FileUnix()
{
	close();
}

void FileUnix::close()
{
	if(_f!=-1)
	{
		::close(_f);
		_f = -1;
	}
}

int64_t FileUnix::size()
{
	// Find the file size
	if(_f==-1)
		E::FileIO("File::size(): File has been closed: %s", _path.full().ptr());

	// Remember the current position
	off_t cur = lseek(_f, 0, SEEK_CUR);

	// Go to the end and mark the position
	off_t end = lseek(_f, 0, SEEK_END);

	// Go back to the original position
	lseek(_f, cur, SEEK_SET);

	// Return the size
	return (int64_t)end;
}

int64_t FileUnix::read(void *ptr, int64_t size)
{
	if(_f==-1)
		E::FileIO("File::read(): File has been closed: %s", _path.full().ptr());

	// Try to read data
	ssize_t r = ::read(_f, ptr, (size_t)size);

	if(r==-1)
		E::FileIO("FIle::read(): Error reading file: %s: %s", _path.full().ptr(), strerror(errno));

	return (int64_t)r;
}

int64_t FileUnix::write(const void *ptr, int64_t size)
{
	if(_f==-1)
		E::FileIO("File::write(): File has been closed: %s", _path.full().ptr());

	if(_access!=FileAccess::Write)
		E::FileIO("File::write(): The file is open read-only: %s", _path.full().ptr());

	// Try to write data
	ssize_t r = ::write(_f, ptr, size);

	if(r==-1)
		E::FileIO("File::write(): Error writing file: %s: %s", _path.full().ptr(), strerror(errno));

	return (int64_t)r;
}

int64_t FileUnix::tell()
{
	if(_f==-1)
		E::FileIO("File::tell(): File has been closed: %s", _path.full().ptr());

	// Get the position
	return (int64_t)lseek(_f, 0, SEEK_CUR);
}

int64_t FileUnix::seek(int64_t pos)
{
	if(_f==-1)
		E::FileIO("File::seek(): File has been closed: %s", _path.full().ptr());

	return (int64_t)lseek(_f, pos, SEEK_SET);
}


