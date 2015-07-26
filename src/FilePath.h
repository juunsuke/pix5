#pragma once


class FilePath
{
	Str _full;
	// Full path

	int _mp_pos, _mp_len;
	int _path_pos, _path_len;
	int _dir_pos, _dir_len;
	int _file_pos, _file_len;
	int _file_base_pos, _file_base_len;
	int _ext_pos, _ext_len;
	// Start position and length of the various components

	bool _valid;
	// Wether the path is valid

	Str _error;
	// Description of why the path is not valid, when it isn't


	void reset()
	{
		// Reset everything
		_full = "";
		_valid = false;
		_error = "";

		_mp_pos = 0;			_mp_len = 0;
		_path_pos = 0;			_path_len = 0;
		_dir_pos = 0;			_dir_len = 0;
		_file_pos = 0;			_file_len = 0;
		_file_base_pos = 0;		_file_base_len = 0;
		_ext_pos = 0;			_ext_len = 0;
	}

	void copy(const FilePath& o)
	{
		_full = o._full;
		_valid = o._valid;
		_error = o._error;

		_mp_pos = o._mp_pos;
		_mp_len = o._mp_len;
		_path_pos = o._path_pos;
		_path_len = o._path_len;
		_dir_pos = o._dir_pos;
		_dir_len = o._dir_len;
		_file_pos = o._file_pos;
		_file_len = o._file_len;
		_file_base_pos = o._file_base_pos;
		_file_base_len = o._file_base_len;
		_ext_pos = o._ext_pos;
		_ext_len = o._ext_len;
	}

	void decode(const Str& s)
	{
		// Decode a string, building a full path out of it and splitting parts
		reset();

		// Special case: Empty string
		if(s.len()==0)
		{
			_full = "";
			_valid = false;
			_error = "Empty file path/name";
			return;
		}

		// Check if there is a mount point provided
		int pos = s.find(':');
		if(pos==-1)
		{
			// No mount point, use "home:" as default
			_full = "home:";
			pos = 0;
		}
		else if(pos==0)
		{
			_valid = false;
			_error = "Empty mount point";
			return;
		}
		else
		{
			// Copy the mount point
			pos++;
			_full = s.sub(0, pos);

			if(pos==s.len())
			{
				_valid = false;
				_error = "No file path/name, only a mount point";
				return;
			}
		}

		_mp_pos = 0;
		_mp_len = _full.len()-1;

		// Add a leading / if the path doesn't have one
		if(s[pos]!='/')
			_full += "/";

		// Append the full path
		_full += s.sub(pos);

		_path_pos = _mp_len+1;
		_path_len = _full.len()-_path_pos;

		// Find the position of the last slash
		int ls;

		for(ls = _full.len()-1; ls>=0; ls--)
			if(_full[ls]=='/')
				break;

		// Mark the dir/file positions
		_dir_pos = _path_pos;
		_dir_len = ls-_path_pos+1;
		_file_pos = ls+1;
		_file_len = _full.len()-_file_pos;

		// Split the file into base and extension
		_file_base_pos = _file_pos;
		_file_base_len = _file_len;

		// Look for an extension
		for(int c = _file_pos+_file_len-1; c>=_file_pos; c--)
			if(_full[c]=='.')
			{
				// Found it
				_file_base_len = c-_file_base_pos;

				_ext_pos = c+1;
				_ext_len = _full.len()-c-1;
				break;
			}

		_valid = true;
	}

public:

	inline FilePath()
	{
		// Empty path
		reset();
	}

	inline FilePath(const FilePath& o)
	{
		// Copy constructor
		copy(o);
	}

	inline FilePath(const Str& s)
	{
		// Decode a string path
		decode(s);
	}

	inline FilePath(const char *s)
	{
		// Decode a string path
		decode(s);
	}

	inline ~FilePath()
	{
	}

	inline FilePath& operator=(const FilePath& o)
	{
		// Copy a path
		copy(o);
		return *this;
	}

	inline FilePath& operator=(const Str& s)
	{
		// Decode a path
		decode(s);
		return *this;
	}

	inline FilePath& operator=(const char *s)
	{
		// Decode a path
		decode(s);
		return *this;
	}

	inline bool valid() const { return _valid; }
	inline Str error() const { return _error; }
	// Return the validity and error



	//
	// The following functions return components of the file path
	// If valid() is false, the values returned by these functions can't be trusted
	// Care should always be taken to make sure the FilePath is valid before extracting its components
	//

	inline Str full() const { return _full; }
	inline operator Str() const { return _full; }
	// Return the full path name

	inline Str mount_point() const { return _full.sub(_mp_pos, _mp_len); }
	// Return the mount point name (exclusing the colon)

	inline Str path() const { return _full.sub(_path_pos, _path_len); }
	// Return the full path section of the file path, including the leading slash

	inline Str dir() const { return _full.sub(_dir_pos, _dir_len); }
	// Return the directory section of the file path, including the terminal slash

	inline Str file() const { return _full.sub(_file_pos, _file_len); }
	// Return the file name section of the file path

	inline Str file_base() const { return _full.sub(_file_base_pos, _file_base_len); }
	// Return the base file name section, that is the filename without its extension

	inline Str ext() const { return _full.sub(_ext_pos, _ext_len); }
	// Return the extension section of the file path, NOT including the leading "."
};

