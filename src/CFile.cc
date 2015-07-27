#include "pix5.h"

namespace PIX {


CFile::CFile(class FileSource *fs, FileAccess::Type access, FILE *f) : File(fs, access)
{
	// Clib file
	_f = f;

	// If the access is Append, the seek to the end of the file
	if(access==FileAccess::Append)
		fseek(f, 0, SEEK_END);
}

CFile::~CFile()
{
	on_close();
}

void CFile::on_close()
{
	// Close the file
	if(_f)
	{
		fclose(_f);
		_f = NULL;
	}
}

int64_t CFile::tell()
{
	// Get the current position
	if(!_f)
		E::ClosedFile("CFile::tell()");

	return ftell(_f);
}

void CFile::seek(int64_t pos)
{
	// Seek to a new position
	if(!_f)
		E::ClosedFile("CFile::seek()");

	fseek(_f, pos, SEEK_SET);
}

int64_t CFile::size()
{
	// Get the file size
	if(!_f)
		E::ClosedFile("CFile::size()");

	int64_t old_pos = ftell(_f);
	fseek(_f, 0, SEEK_END);
	int64_t sz = ftell(_f);
	fseek(_f, old_pos, SEEK_SET);

	return sz;
}
	
size_t CFile::read(void *buf, size_t size)
{
	if(size<=0)
		return 0;

	// Read data
	clearerr(_f);
	size_t r = fread(buf, 1, size, _f);

	// If r is >0, at least some data was read, so it was a success
	if(r>0)
		return r;

	// If 0 was returned, it could mean an error
	int e = ferror(_f);
	if(e)
		E::ReadFile("Error while reading from file: %s", strerror(e));

	// No error, simple reached EOF
	return 0;
}


}
