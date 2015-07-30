#include "pix5.h"
#include "data.h"


bool run = true;

class MyEventHandler: public EventHandler
{
public:

	void on_mouse_wheel_down(Mouse& mouse)
	{
	}

	void on_mouse_wheel_up(Mouse& mouse)
	{
	}

	void on_mouse_move(Mouse& mouse)
	{
	}

	void on_key_down(Key& key)
	{
		if(key==KEY_1)
			Display::set_renderer(RendererType::Immediate);
		if(key==KEY_2)
			Display::set_renderer(RendererType::VBO);
		if(key==KEY_3)
			Display::set_renderer(RendererType::Shader);
	}

	void on_key_up(Key& key)
	{
	}

	void on_text(const Str& text)
	{
	}

	void on_window_resize(int width, int height)
	{
	}

	void on_quit()
	{
		run = false;
	}
};



int main(int argc, char **argv)
{
	try
	{
		pix_init("Test");

		Display::set_mode(VideoMode::resizable(
			1366, 768, false
		));


		Texture *tex = Cache::texture("data/konata.png")->sub(0, 0, 16, 16);


		MyEventHandler eh;

		float a = 0;

		while(run)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));

			tex->draw(100, 10);

			for(int c = 0; c<16000; c++)
				tex->draw(rand()%1000, rand()%1000);
		
			tex->draw(Input::get_mouse().x, Input::get_mouse().y, tex->width()/2, tex->height()/2, a, 1, 1);
			tex->draw(Input::get_mouse().x+100, Input::get_mouse().y+100, Color(1, 0, 0), Color(0, 1, 0), Color(0, 0, 1), Color(1, 1, 1));


			a += 1.0f;


			Display::render();

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


