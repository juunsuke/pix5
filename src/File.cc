#include "pix5.h"

namespace PIX {



File::File(FileSource *fs, FileAccess::Type access)
{
	_fs = fs;
	_access = access;
}

File::~File()
{
}



}
