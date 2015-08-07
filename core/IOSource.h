#pragma once


class IOSource
{
protected:

	IOSource();

public:

	virtual ~IOSource();


	virtual Str get_desc() = 0;
	// Return a descriptive string for the source


	
	//
	// Virtual events
	//

	void on_mount(const Str& mount_point) {};
	// Called when the source is being mounted on the specified mount point
	// The source is not yet mounted when this is called
	// If an error is thrown, the mount will be aborted

	void on_unmount(const Str& mount_point, bool force) {};
	// Called when the source is requested to unmount
	// If an error is thrown and 'force' is false, the mount is aborted
	// If 'force' is true, the source should prepare for a guaranteed unmounting,
	// even if an error is thrown, the source will be forcibly deleted

	virtual File *on_open(const FilePath& path, FileAccess::Type access) = 0;
	// Open a file

	virtual void on_get_files(const FilePath& path, List<FileEntry*> &files) {}
	// Get the list of files in the given directory
};

