#pragma once


class MountEntry
{
public:

	Str mount_point;
	// The name of the mount point

	Str desc;
	// The description provided by the IO source


	MountEntry(const Str& mount_point, const Str& desc)
	{
		this->mount_point = mount_point;
		this->desc = desc;
	}

	~MountEntry()
	{
	}
};

