#pragma once


namespace GZip
{



Buffer<char> *compress(const void *ptr, int size, int level = -1);
Buffer<char> *compress(const Buffer<char> &buf, int level = -1);
// Compress data using zlib
// If the compressed data would be larger than the source data, NULL is returned
// The returned buffer is caller-owned and should be deleted when processed

Buffer<char> *decompress(const void *ptr, int size);
Buffer<char> *decompress(const Buffer<char> &buf);
// Decompress data that was compressed using compress()
// An error will be thrown if the input buffer is incomplete or erroneous
// The returned buffer is caller-owned and should be deleted when processed



}
