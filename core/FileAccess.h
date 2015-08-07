#pragma once


namespace FileAccess
{
	enum Type
	{
		Read,
		// Read-only access

		Write,
		// Read/write access

		Create,
		// Create the file if it doesn't exist and open it for read/write
		// If it exists, open for read/write only

		Truncate,
		// Create the file if it doesn't exist and open it for read/write
		// If it exists, truncate its contents and open for read/write
	};
}

