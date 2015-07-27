#pragma once


class CFile: public File
{
	FILE *_f;
	// File handle


public:

	CFile(class FileSource *fs, FileAccess::Type access, FILE *f);
	~CFile();


	//
	// Overloaded functions from File
	//

	int64_t tell();
	void seek(int64_t pos);
	int64_t size();
	size_t read(void *buf, size_t size);
	void on_close();
};



