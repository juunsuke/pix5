#pragma once


class ImmRenderer: public Renderer
{
	void on_init();
	void on_done();
	void on_render(int start, int num);

public:

	ImmRenderer();
	~ImmRenderer();

};

