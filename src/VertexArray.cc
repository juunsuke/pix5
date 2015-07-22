#include "pix5.h"
#include <GL/glew.h>


namespace PIX {



VertexArray::VertexArray()
{
	_vao = 0;
	_size = 0;
}

VertexArray::~VertexArray()
{
	// Clear the streams and the VAO
	clear_streams();
}

void VertexArray::delete_gl()
{
	if(_vao)
	{
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}

	_size = 0;

	// Do the same for the vertex buffers
	for(int c = 0; c<_vb.size(); c++)
		_vb[c]->delete_gl();
}

int VertexArray::add_stream(const VertexDef& def, int stride, VertexBufferUsage::Type usage)
{
	// Create a new vertex buffer
	VertexBuffer *vb = new VertexBuffer(def, stride, usage, false);

	// Add it and return its index
	return _vb.add(vb);
}

void VertexArray::clear_streams()
{
	// Clear the streams
	_vb.clear_del();

	// Delete the VAO
	delete_gl();
}

void VertexArray::create_gl(Shader *shad)
{
	// Create the VAO
	glGenVertexArrays(1, &_vao);

	// Bind it
	glBindVertexArray(_vao);

	// Create, bind and enable all the vertex buffers
	for(int c = 0; c<_vb.size(); c++)
	{
		VertexBuffer *vb = _vb[c];

		// Create/bind
		vb->bind();

		// Setup the attribs
		vb->set_attribs(shad);
	}
}
	
void *VertexArray::lock(int stream, int first, int count)
{
	if(stream<0 || stream>=_vb.size())
		E::BadStreamIndex("VertexArray::lock()");

	if((first+count)>_size && _vao)
		delete_gl();

	if((first+count)>_size)
		_size = first+count;

	// Lock the VB
	return _vb[stream]->lock(first, count);
}

void VertexArray::bind(Shader *shad)
{
	// Create the VAO and VBOs if not created
	if(!_vao)
	{
		create_gl(shad);
		return;
	}

	// Bind the VAO
	glBindVertexArray(_vao);

	// Update dirty VBs
	for(int c = 0; c<_vb.size(); c++)
	{
		VertexBuffer *vb = _vb[c];

		if(vb->is_dirty())
			vb->bind();
	}
}

void VertexArray::unbind()
{
	// Unbind any VAO
	glBindVertexArray(0);
}

}
