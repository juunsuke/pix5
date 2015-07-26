#include "pix5.h"

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
}

LinuxFileSource::~LinuxFileSource()
{
}

}
