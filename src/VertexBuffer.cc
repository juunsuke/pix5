#include "pix5.h"
#include <GL/glew.h>

namespace PIX {

/*

VertexBuffer::VertexBuffer(const VertexDef& def, int stride, VertexBufferUsage::Type usage, bool double_buf)
{
	_size = 0;
	_data = NULL;
	_stride = 0;
	_usage = usage;
	_vbo[0] = 0;
	_vbo[1] = 0;
	_double_buf = double_buf;
	_cur_vbo = 0;

	set_def(def, stride);
}

VertexBuffer::~VertexBuffer()
{
	clear_data();
	delete_gl();
}

void VertexBuffer::delete_gl()
{
	if(_vbo[0])
	{
		glDeleteBuffers(1, &_vbo[0]);
		_vbo[0] = 0;
	}
	if(_vbo[1])
	{
		glDeleteBuffers(1, &_vbo[1]);
		_vbo[1] = 0;
	}
}

void VertexBuffer::clear_data()
{
	// Clear the data buffer
	if(_data)
		free(_data);

	_data = NULL;
	_size = 0;
}

void VertexBuffer::set_def(const VertexDef& def, int stride)
{
	// Clear any data
	clear_data();

	// Validate and set the definition
	if(def.validate()>stride)
		E::BadVertexDef("VertexBuffer::set_def(): The definition is larger than the provided stride");

	// All seems fine
	_def = def;
	_stride = stride;
}
	
void VertexBuffer::set_usage(VertexBufferUsage::Type usage)
{
	if(usage==_usage)
		return;

	// Mark the new usage
	_usage = usage;

	// Destroy the VBO if it exists
	delete_gl();
}

void VertexBuffer::resize(int size)
{
	if(size==_size)
		return;

	if(size<=0)
	{
		clear_data();
		return;
	}

	// Resize
	_size = size;
	_data = realloc(_data, size*_stride);

	// Destroy the VBO
	delete_gl();

	_dirty = true;
}

void *VertexBuffer::lock(int first, int count)
{
	ASSERT(first>=0, "VertexBuffer::lock(): Invalid first index")

	// Make sure the buffer is large enough
	int sz = first+count;
	if(sz>_size)
		resize(sz);

	// Always mark as dirty
	_dirty = true;

	// Return the pointer
	return ((char*)_data)+first*_stride;
}

void VertexBuffer::create_gl(int i)
{
	// Create the VBO
	glGenBuffers(1, &_vbo[i]);

	// Bind it
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);

	// Upload the data
	glBufferData(GL_ARRAY_BUFFER, _size*_stride, _data, get_usage(_usage));
}

void VertexBuffer::create_gl()
{
	// Create the VBOs
	if(_double_buf)
	{
		// The first time the VBOs are created, upload the data twice
		// This is to make sure both buffers are defined
		create_gl(1);
		create_gl(0);
	}
	else
	{
		// Just use the one VBO
		create_gl(0);
	}

	_cur_vbo = 0;
	_dirty = false;
}

void VertexBuffer::bind()
{
	ASSERT(Display::get_window(), "VertexBuffer::bind(): No mode currently set")

	// Create the VBOs if it they don't exist
	if(!_vbo[0])
	{
		create_gl();
		return;
	}

	// If dirty and using double buffering, swap the buffer we'll use
	if(_double_buf && _dirty)
		_cur_vbo = (_cur_vbo==0) ? 1 : 0;

	// Bind it
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[_cur_vbo]);

	// Re-upload the data if dirty
	if(_dirty)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, _size*_stride, _data);
		_dirty = false;
	}
}
	
void VertexBuffer::unbind()
{
	ASSERT(Display::get_window(), "VertexBuffer::unbind(): No mode currently set")
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::set_attribs(Shader *shad)
{
	ASSERT(Display::get_window(), "VertexBuffer::set_attribs(): No mode currently set")

	// Set all ther vertrex attrib pointers
	int num = _def.num_comp();
	for(int c = 0; c<num; c++)
	{
		const VertexComp *comp = _def.get_comp(c);

		// Retrieve the index from the shader
		int i = shad->get_attribute_index(comp->name);
		if(i==-1)
			// No such attribute, skip it
			continue;

		// Setup the pointer
		glVertexAttribPointer(i, comp->size, comp->type, comp->norm, _stride, ((char*)NULL)+comp->offset);

		//Log::debug("%i: Size:%i   Type:%i   Norm:%i   Stride:%i   Offset:%i   Name: %s", i, comp->size, comp->type, comp->norm, _stride, comp->offset, comp->name.ptr());

		// Enable it
		glEnableVertexAttribArray(i);
	}
}

void VertexBuffer::disable_attribs()
{
	// Get the max number of attribs
	int num;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);

	// Disable them all
	for(int c = 0; c<num; c++)
		glDisableVertexAttribArray(c);
}

*/
}
