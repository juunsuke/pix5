#include "pix5.h"

namespace PIX {



File::File(FileSource *fs, FileAccess::Type access)
{
	_fs = fs;
	_access = access;
}

File::~File()
{
	// Close the file
	close();
}

void File::close()
{
	// Perform the actual closing
	on_close();

	// Let the FS know
	_fs->on_close(this);
}

size_t File::read_all(void **buf)
{
	// Get the size
	size_t sz = size();

	// Move to the start
	seek(0);

	// Allocate a buffer
	*buf = malloc(sz+1);

	// Read the data
	try
	{
		size_t r = read(*buf, sz);
		if(r!=sz)
			E::ReadFile("File::read_all(): Unexpected EOF");
	}
	catch(Error)
	{
		free(buf);
		throw;
	}

	// Append a 0 for convinience
	((char*)(*buf))[sz] = 0;

	return sz;
}


}
