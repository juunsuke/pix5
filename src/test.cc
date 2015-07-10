#include "pix5.h"


Texture *tex = NULL;

class MyEventHandler: public EventHandler
{
public:

	void on_mouse_wheel_down(Mouse& mouse)
	{
		printf("Down!\n");
	}

	void on_mouse_wheel_up(Mouse& mouse)
	{
		printf("Up!\n");

		Input::move_mouse(10,10);
	}

	void on_mouse_move(Mouse& mouse)
	{
		//printf("%i, %i\n", mouse.x, mouse.y);
	}

	void on_key_down(Key& key)
	{
		printf("DOWN: %s\n", key.key_name.ptr());
	}

	void on_key_up(Key& key)
	{
		printf("  UP: %s\n", key.key_name.ptr());
	}

	void on_text(const Str& text)
	{
		printf("TEXT: %s\n", text.ptr());
	}

	void on_window_resize(int width, int height)
	{
	}
};


typedef struct VertexA
{
	float x, y;
	uint32_t col;
} VertexA;

typedef struct VertexB
{
	float u, v;
} VertexB;


int main(int argc, char **argv)
{
	try
	{
		pix_init("Test");

		VertexDef vd;
		vd.add(VertexComp::float2("pos", 0));
		vd.add(VertexComp::ubyte4("col", true, 8));

		VertexArray *va = new VertexArray();
		va->add_stream(vd, sizeof(VertexA));
		
		vd.clear();
		vd.add(VertexComp::float2("texcoord", 0));

		va->add_stream(vd, sizeof(VertexB));

		VertexA *v = (VertexA*)va->lock(0, 0, 4);

		v[0].x = 100;
		v[0].y = 10;
		v[0].col = 0xFFFF0000;
		
		v[1].x = 591;
		v[1].y = 10;
		v[1].col = 0xFF00FF00;

		v[2].x = 591;
		v[2].y = 550;
		v[2].col = 0xFF0000FF;

		v[3].x = 100;
		v[3].y = 550;
		v[3].col = 0xFF808080;


		VertexB *v2 = (VertexB*)va->lock(1, 0, 4);
		
		v2[0].u = 0;
		v2[0].v = 0;

		v2[1].u = 1;
		v2[1].v = 0;

		v2[2].u = 1;
		v2[2].v = 1;

		v2[3].u = 0;
		v2[3].v = 1;


		Display::set_mode(VideoMode::resizable());

		tex = Texture::load("konata.png");
		tex->bind(0);

		Shader *shad = Shader::load("Shader2D", "shader2d.vs", "shader2d.fs");

		MyEventHandler eh;


		Font *fnt = new Font("data/LiberationSans-Regular.ttf", 12);
		fnt->get_glyph('A');

		fnt->print(tex, 10, 50, Color(1, 1, 0), "Proutentakendok");
		tex->bind(0);

		for(;;)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));
			//Display::clear();
		
			Display::test_draw(600, 100, tex);

			shad->bind();
			shad->set_uniform("tex", 0);
			
	
			Display::get_2d_camera().set();
		

			va->bind(shad);

			Display::draw(0, 4);

			VertexArray::unbind();
			Shader::unbind();


			//Display::test_draw(100, 10, 200, 50, NULL, Color(0.9f, 0.3f, 0.4f));


			Display::swap();

			eh.process_events();
		}

		pix_shutdown();
	}
	catch(Error e)
	{
		printf("Error: %s\n", e.format().ptr());
	}

	return 0;
}


