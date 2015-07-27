#pragma once


class FileSource
{
protected:

	FileSource();

	virtual void on_close(class File *file) {}
	// Called by File::close() when a file is being closed

	friend class File;

public:

	virtual ~FileSource();

	virtual Str get_info_str() = 0;
	// Return an information string about the file source


	virtual File *open_file(const FilePath& fname, FileAccess::Type access) = 0;
	// Open the file represented by 'fname'
	// The file path is guaranteed to be valid
	// Throw an error if the file can't be opened using the requested access

	virtual void on_mount(const Str& mount_point) {}
	// Called after the file source has been pointed on the given mount point
	// If any error is thrown, the mount will be aborted and the error re-thrown
	// When the mount is aborted due to an error, the filesource will be forcefully
	// unmounted, so no on_unmount() event will be triggered.  However, an on_force_unmount()
	// will be triggered

	virtual void on_unmount(const Str& mount_point) {}
	// Called when the file source is being unmounted from the given mount point
	// This is only called when a graceful unmount is being done

	virtual void on_force_unmount(const Str& mount_point) {}
	// Called when the file source is being forcefully unmounted
	// It will be called when an error is thrown during mounting, or at shutdown
	// if the file source can't be unmounted normally, like for instance if it still
	// has files open within it

};


