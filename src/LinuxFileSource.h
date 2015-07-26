#pragma once


class LinuxFileSource: public FileSource
{
	Str _path;
	// Base path

public:

	LinuxFileSource(const Str& path);
	~LinuxFileSource();

};

