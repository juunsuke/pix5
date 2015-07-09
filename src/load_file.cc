#include "pix5.h"

namespace PIX {


int load_file(const Str& fname, char **buf)
{
	// Try and open the file
	FILE *f = fopen(fname, "rb");
	if(!f)
		return -1;

	// Find out its size
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	fseek(f, 0, SEEK_SET);

	// Allocate a buffer
	*buf = (char*)malloc(sz+1);

	// Read the file
	if(fread(*buf, sz, 1, f)!=1)
	{
		// Error
		fclose(f);
		free(*buf);
		*buf = NULL;
		return -1;
	}

	// Add a terminal 0
	(*buf)[sz] = 0;

	// All done
	fclose(f);
	return sz;
}

}
