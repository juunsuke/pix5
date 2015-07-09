#pragma once


class Error
{
public:

	uint32_t code;
	// Error code

	Str name;
	// Error name

	Str desc;
	// Generic error description

	Str message;
	// Specific error message


	Error(uint32_t code, const Str& name, const Str &desc, const Str& message);
	// Defined in the CC file because it calls log in debug mode

	Error(const Error& o)
	{
		// Copy constructor
		code = o.code;
		name = o.name;
		desc = o.desc;
		message = o.message;
	}

	~Error()
	{
	}

	Error& operator=(const Error& o)
	{
		// Affectation
		code = o.code;
		name = o.name;
		desc = o.desc;
		message = o.message;

		return *this;
	}

	bool operator==(const Error& o) const
	{
		// Compare the codes only
		return (code==o.code);
	}

	bool operator==(uint32_t code) const
	{
		// Compare codes
		return (this->code==code);
	}

	Str format() const
	{
		// Build a formatted string for the error
		return Str::build("%s [0x%08X] (%s): %s", name.ptr(), code, desc.ptr(), message.ptr());
	}
};


