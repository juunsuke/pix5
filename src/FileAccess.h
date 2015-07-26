#pragma once


namespace FileAccess
{
	enum Type
	{
		ReadOnly,
		// Read-only access

		ReadWrite,
		// Read/write access

		Append,
		// Read/write access
		// Position the pointer at the end of the file after opening it

		Create,
		// Read/write access
		// Create the file if it doesn't exist

		Purge,
		// Read/write access
		// Create the file if it doesn't exist
		// Empty its content if it exists
	};
}


