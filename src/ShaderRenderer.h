#pragma once


class ShaderRenderer: public VBORenderer
{
	uint32_t _vs, _fs, _prg;
	// The vertex and fragment shaders, and the program

	uint32_t compile(int type, char *src, int len);
	// Compile a shader

	void create_prog();
	// Create the program


	void on_init();
	void on_done();
	void on_prepare();
	void on_finish();

public:

	ShaderRenderer();
	~ShaderRenderer();

};

