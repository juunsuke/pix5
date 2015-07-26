#pragma once


class LinuxFileSource: public FileSource
{
	Str _path;
	// Base path

	Str get_info_str();

	File *open_file(const FilePath& fname, FileAccess::Type access);

public:

	LinuxFileSource(const Str& path);
	~LinuxFileSource();
	
};

