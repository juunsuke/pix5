#pragma once


class File
{
protected:

	File()
	{
	}

public:

	virtual ~File()
	{
	}


	//
	// Overloadable functions for file access
	//

	virtual void close() = 0;
	// Close the file

	virtual int64_t size() = 0;
	// Return the file size

	virtual int64_t read(void *ptr, int64_t size) = 0;
	// Read up to 'size' bytes and store them at the address pointed to by 'ptr'
	// Returns the number of bytes actually read

	virtual int64_t write(const void *ptr, int64_t size) = 0;
	// Write up to 'size' bytes from the given buffer
	// Returns the number of bytes actually written

	virtual int64_t tell() = 0;
	// Retrieve the current file position

	virtual int64_t seek(int64_t pos) = 0;
	// Seek to the given position
};

