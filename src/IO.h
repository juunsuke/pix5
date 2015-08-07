#pragma once

namespace IO
{



void mount(const Str& mount_point, FileSource *fs);
// Mount the given file source on the given mount point
// The IO sub-system takes ownership of the file source, and upon
// unmounting it, will delete it
// If an error occurs while trying to mount, the filesource will NOT be deleted

void unmount(const Str& mount_point);
// Unmount the filesource mounted at the given mount point
// If any file is still open within that file source, an error will be thrown
// The filesource will be deleted

File *open(const FilePath& fname, FileAccess::Type access = FileAccess::ReadOnly);
// Open the file with the given name using the given access
// Throws an error if the file can't be opened

size_t read_file(const FilePath& fname, void **buf);
// Open the file referenced to by 'fname', allocate a buffer large enough to
// hold the whole file and read it all
// The buffer's address is set at the address pointed to by 'buf'
// Returns the file size


}
