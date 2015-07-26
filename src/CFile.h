#pragma once


class CFile: public File
{
	FILE *_f;
	// File handle

public:

	CFile(class FileSource *fs, FileAccess::Type access, FILE *f);
	~CFile();
};



