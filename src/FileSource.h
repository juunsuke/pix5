#pragma once


class FileSource
{
protected:

	FileSource();

public:

	virtual ~FileSource();


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


