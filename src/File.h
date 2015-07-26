#pragma once


class File
{
	class FileSource *_fs;
	// The filesource that owns this file

	FileAccess::Type _access;
	// Access type

protected:
	
	File(FileSource *fs, FileAccess::Type access);

public:

	virtual ~File();
};

