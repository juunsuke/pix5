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





}
