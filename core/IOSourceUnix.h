#pragma once


class IOSourceUnix: public IOSource
{
	Str _base;
	// Base path

public:

	IOSourceUnix(const Str& base_path);
	~IOSourceUnix();


	virtual Str get_desc()
	{
		return Str::build("Linux/Unix IO source for %s", _base.ptr());
	}
	
	
	void on_mount(const Str& mount_point);
	File *on_open(const FilePath& path, FileAccess::Type access);
	void on_get_files(const FilePath& path, List<FileEntry*> &files);

};

