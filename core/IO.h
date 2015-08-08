#pragma once

namespace IO
{



void mount(const Str& mount_point, IOSource *src);
// Mount the provided I/O source at the given mount point
// Upon success, PIX takes ownership of 'src', and will delete it
// when it gets unmounted
// Upon failure, an error is thrown and 'src' is not deleted

void unmount(const Str& mount_point, bool force = false);
// Unmount the given mount point
// If 'force' is true, the mount will be forcibly unmounted, even
// if the source doesn't want a clean unmount (i.e. if it still
// has open files)

File *open(const FilePath& path, FileAccess::Type access = FileAccess::Read);
// Open a file
// Throws an error on failure

Buffer<char> *read_file(const FilePath& path, bool add_zero = false);
// Read a whole file
// If 'path' is successfully open for reading, a new buffer will be allocated
// and the contents of the file will be read into it
// If 'add_zero' is set to true, a terminal 0 character will be added at the
// end of the buffer, which will be included in the buffer size
// The returned buffer is caller-owned, and should be deleted when processed

void get_files(const FilePath& path, List<FileEntry*> &files);
// Retrieve the list of files in the directory pointed to by 'path'
// If the path is valid, 'files' will be filled with file entries
// The entries are caller-owned and should be deleted when processed
// The special directories '.' and '..' are never returned

void get_mounts(List<MountEntry*> &mounts);
// Retrieve a list of all currently mounted IO sources
// The entries are caller-owned and should be deleted when processed



}
