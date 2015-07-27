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


	void close();
	// Close the file

	size_t read_all(void **buf);
	// Allocate a buffer large enough to hold all the file and read it all
	// The buffer's address is stored at the address pointed to by buf
	// Returns the size of the data
	// For convinience, a terminal 0 is always appended at the end of the
	// buffer, and not accounted for in the returned size



	virtual int64_t tell() = 0;
	// Retrieve the current file position

	virtual void seek(int64_t pos) = 0;
	// Change the current file position

	virtual int64_t size() = 0;
	// Retrieve the size of the file

	virtual size_t read(void *buf, size_t size) = 0;
	// Read at most 'size' bytes from the file, storing the data into 'buf'
	// Returns 0 if there is no data to read
	// Throw errors if an error occur



	virtual void on_close() { }
	// Called when the file should be closed
};

