#include "pix5core.h"
#include <zlib.h>


namespace GZip
{




Buffer<char> *compress(const void *ptr, int size, int level)
{
	// Prepare the stream
	z_stream zs;
	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;

	if(deflateInit(&zs, level)!=Z_OK)
		E::ZLib("deflateInit() failed");

	// Prepare a buffer
	Buffer<char> *buf = new Buffer<char>(size);

	zs.next_in = (uint8_t*)ptr;
	zs.avail_in = size;
	zs.next_out = (uint8_t*)buf->ptr();
	zs.avail_out = size;

	// Deflate data
	int r = deflate(&zs, Z_FINISH);
	int ao = zs.avail_out;

	// Release the stream
	deflateEnd(&zs);

	// Check if compression has terminated and was successful
	if(r==Z_STREAM_END)
	{
		// Yes, resize and return th buffer
		buf->resize(size-ao);
		return buf;
	}

	// Something happened
	delete buf;

	if(r!=Z_OK)
		E::ZLib("deflate() failed");

	// If we got here, it means the output would have been larger than the input
	return NULL;
}

Buffer<char> *compress(const Buffer<char> &buf, int level)
{
	return compress(buf.ptr_const(), buf.size(), level);
}


Buffer<char> *decompress(const void *ptr, int size)
{
	// Prepare the stream
	z_stream zs;
	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;
	zs.next_in = (uint8_t*)ptr;
	zs.avail_in = size;

	if(inflateInit(&zs)!=Z_OK)
		E::ZLib("inflateInit() failed");

	int osize = size;
	int pos = 0;
	Buffer<char> *buf = new Buffer<char>(osize);

	// Decompress the data
	for(;;)
	{
		// Run an inflate round
		zs.next_out = (uint8_t*)(buf->ptr()+pos);
		zs.avail_out = size;

		int r = inflate(&zs, Z_NO_FLUSH);

		// Done decompressing ?
		if(r==Z_STREAM_END)
			break;

		// Error ?
		if(r!=Z_OK)
		{
			inflateEnd(&zs);
			delete buf;
			E::ZLib("inflate() failed, most likely invalid compressed data");
		}

		// Enlarge the buffer and keep inflating
		pos += size-zs.avail_out;
		osize = pos+size;
		buf->resize(osize);
	}

	// Decompression finished successfully
	// Adjust the buffer size
	osize -= zs.avail_out;
	buf->resize(osize);

	inflateEnd(&zs);

	return buf;
}

Buffer<char> *decompress(const Buffer<char> &buf)
{
	return decompress(buf.ptr_const(), buf.size());
}



}
