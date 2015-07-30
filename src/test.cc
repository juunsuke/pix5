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

		Lua lua;
		lua.load_file("test.lua");

		MyEventHandler eh;

		while(run)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));


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


