#include "pix5gfx.h"
#include "bi_data.h"
#include <GL/glew.h>


ShaderRenderer::ShaderRenderer()
{
	_vs = 0;
	_fs = 0;
}

ShaderRenderer::~ShaderRenderer()
{
}

uint32_t ShaderRenderer::compile(int type, char *src, int len)
{
	// Create a new shader
	uint32_t sh = glCreateShader(type);

	// Provide the source
	glShaderSource(sh, 1, (const char**)&src, &len);

	// Compile it
	glCompileShader(sh);

	// Check the result
	int status;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
	if(!status)
	{
		// Failure, get the reason
		int len;
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);

		Buffer<char> buf(len);
		glGetShaderInfoLog(sh, len, &len, buf.ptr());

		// Destroy the shader and throw an error
		glDeleteShader(sh);

		E::Renderer("Error compiling shader: %s", buf.ptr());
	}

	// Compilation was successful
	return sh;
}

void ShaderRenderer::create_prog()
{
	// Create a program
	_prg = glCreateProgram();

	// Attach the shaders
	glAttachShader(_prg, _vs);
	glAttachShader(_prg, _fs);

	// Link it
	glLinkProgram(_prg);

	// Check status
	int status;
	glGetProgramiv(_prg, GL_LINK_STATUS, &status);
	if(!status)
	{
		// Failure, get the reason
		int len;
		glGetProgramiv(_prg, GL_INFO_LOG_LENGTH, &len);

		Buffer<char> buf(len);
		glGetProgramInfoLog(_prg, len, &len, buf.ptr());

		// Throw an error
		E::Renderer("Error linking shaders: %s", buf.ptr());
	}
}

void ShaderRenderer::on_init()
{
	// Compile and link the shaders
	try
	{
		_vs = compile(GL_VERTEX_SHADER, (char*)data_vs_glsl, data_vs_glsl_len);
		_fs = compile(GL_FRAGMENT_SHADER, (char*)data_fs_glsl, data_fs_glsl_len);
		create_prog();
	}
	catch(Error)
	{
		on_done();
		throw;
	}

	// Init the VBO
	VBORenderer::on_init();
}

void ShaderRenderer::on_done()
{
	// Shutdown the VBO
	VBORenderer::on_done();

	// Delete the shaders
	if(_vs)
	{
		glDeleteShader(_vs);
		_vs = 0;
	}

	if(_fs)
	{
		glDeleteShader(_fs);
		_fs = 0;
	}
}

void ShaderRenderer::on_prepare()
{
	// Use the program
	glUseProgram(_prg);

	VBORenderer::on_prepare();
}

void ShaderRenderer::on_finish()
{
	VBORenderer::on_finish();

	// Stop using the program
	glUseProgram(0);
}


