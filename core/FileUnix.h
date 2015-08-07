#pragma once


class FileUnix: public File
{
	FilePath _path;
	// File's path

	FileAccess::Type _access;
	// Open acces mode

	int _f;
	// File handle

public:

	FileUnix(const FilePath& path, FileAccess::Type access, int f);
	~FileUnix();


	//
	// Overloaded functions
	//

	void close();
	int64_t size();
	int64_t read(void *ptr, int64_t size);
	int64_t write(const void *ptr, int64_t size);
	int64_t tell();
	int64_t seek(int64_t pos);
};

