#include "pix5.h"

namespace PIX {


CFile::CFile(class FileSource *fs, FileAccess::Type access, FILE *f) : File(fs, access)
{
	// Clib file
	_f = f;
}

CFile::~CFile()
{
}




}
