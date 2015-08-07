#pragma once


class FileEntry
{
public:

	FilePath path;
	// Full path of the file

	bool dir;
	// Wether the file is a directory

	int64_t size;
	// File size


	FileEntry(const FilePath& path, bool dir, int64_t size)
	{
		this->path = path;
		this->dir = dir;
		this->size = size;
	}

	FileEntry(const FileEntry& o)
	{
		path = o.path;
		dir = o.dir;
		size = o.size;
	}

	~FileEntry()
	{
	}


	FileEntry& operator=(const FileEntry& o)
	{
		path = o.path;
		dir = o.dir;
		size = o.size;
		return *this;
	}
};

