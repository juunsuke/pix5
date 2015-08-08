#include "pix5net.h"


NetMessage::NetMessage()
{
	_pos = 0;
	_size = 0;
}

NetMessage::~NetMessage()
{
}

void NetMessage::copy(const NetMessage& o)
{
	_buf = o._buf;
	_size = o._size;
	_pos = o._pos;
}
	
void NetMessage::add_size(int sz)
{
	// Resize the buffer ?
	if((_size+sz)>_buf.size())
		_buf.resize(_size+sz+1024);

	// Increase the size
	_size += sz;
}
	
void NetMessage::add_i8(int8_t val)			{	add_size(1);	*(int8_t*)&_buf[_pos] = val;	_pos++;			}
void NetMessage::add_u8(uint8_t val)		{	add_size(1);	*(uint8_t*)&_buf[_pos] = val;	_pos++;			}
void NetMessage::add_i16(int16_t val)		{	add_size(2);	*(int16_t*)&_buf[_pos] = val;	_pos += 2;		}
void NetMessage::add_u16(uint16_t val)		{	add_size(2);	*(uint16_t*)&_buf[_pos] = val;	_pos += 2;		}
void NetMessage::add_i32(int32_t val)		{	add_size(4);	*(int32_t*)&_buf[_pos] = val;	_pos += 4;		}
void NetMessage::add_u32(uint32_t val)		{	add_size(4);	*(uint32_t*)&_buf[_pos] = val;	_pos += 4;		}
void NetMessage::add_i64(int64_t val)		{	add_size(8);	*(int64_t*)&_buf[_pos] = val;	_pos += 8;		}
void NetMessage::add_u64(uint64_t val)		{	add_size(8);	*(uint64_t*)&_buf[_pos] = val;	_pos += 8;		}
void NetMessage::add_float(float val)		{	add_size(4);	*(float*)&_buf[_pos] = val;		_pos += 4;		}
void NetMessage::add_double(double val)		{	add_size(8);	*(double*)&_buf[_pos] = val;	_pos += 8;		}

void NetMessage::add_str(const Str& val)
{
	int len = val.len();
	if(len>65535)
		E::NetMessage("NetMessage::add_str(): Maximum string length is 65535 bytes");

	add_u16((uint16_t)len);
	add_raw(val.ptr(), len);
}

void NetMessage::add_raw(const void *ptr, int size)
{
	add_size(size);
	memcpy(_buf+_pos, ptr, size);
	_pos += size;
}


void NetMessage::has_size(int sz)
{
	if((_pos+sz)>_size)
		E::NetMessage("Unexpected end of message");
}

bool NetMessage::end()
{
	return _pos==_size;
}

int8_t NetMessage::get_i8()				{	has_size(1);	_pos += 1;		return *(int8_t*)   &_buf[_pos-1];		}
uint8_t NetMessage::get_u8()			{	has_size(1);	_pos += 1;		return *(uint8_t*)  &_buf[_pos-1];		}
int16_t NetMessage::get_i16()			{	has_size(2);	_pos += 2;		return *(int16_t*)  &_buf[_pos-2];		}
uint16_t NetMessage::get_u16()			{	has_size(2);	_pos += 2;		return *(uint16_t*) &_buf[_pos-2];		}
int32_t NetMessage::get_i32()			{	has_size(4);	_pos += 4;		return *(int32_t*)  &_buf[_pos-4];		}
uint32_t NetMessage::get_u32()			{	has_size(4);	_pos += 4;		return *(uint32_t*) &_buf[_pos-4];		}
int64_t NetMessage::get_i64()			{	has_size(8);	_pos += 8;		return *(int64_t*)  &_buf[_pos-8];		}
uint64_t NetMessage::get_u64()			{	has_size(8);	_pos += 8;		return *(uint64_t*) &_buf[_pos-8];		}
float NetMessage::get_float()			{	has_size(4);	_pos += 4;		return *(float*)    &_buf[_pos-4];		}
double NetMessage::get_double()			{	has_size(8);	_pos += 8;		return *(double*)   &_buf[_pos-8];		}

Str NetMessage::get_str()
{
	int len = get_u16();
	has_size(len);
	_pos += len;

	return Str((char*)_buf.ptr()+_pos-len, len);
}

void NetMessage::get_raw(void *ptr, int size)
{
	has_size(size);
	memcpy(ptr, _buf+_pos, size);
	_pos += size;
}




