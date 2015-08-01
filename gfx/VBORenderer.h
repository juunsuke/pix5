#pragma once


class VBORenderer: public Renderer
{
	uint32_t _vbo[2];
	// The two VBOs

	int _size[2];
	// The size of each VBO, in number of quad vertices

	int _cur_vbo;
	// VBO index currently in use


protected:
	void on_init();
	void on_done();
	void on_prepare();
	void on_finish();
	void on_render(int start, int num);

public:

	VBORenderer();
	~VBORenderer();

};


