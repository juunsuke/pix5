#include "pix5.h"


static bool _run = true;



class MyHandler: public GfxEventHandler
{
public:

	void on_quit()
	{
		_run = false;
	}

	void on_key_down(Key &key)
	{
		if(key==KEY_SPACE)
		{
			Display::set_mode(VideoMode::resizable());
		}
	}

};


int main(int argc, char **argv)
{
	pix_init("Test");

	Display::set_mode(VideoMode::resizable(
		//1366, 768, false
	));

	MyHandler mh;

	Texture *tex = Texture::cache("src/data/konata.png");


	while(_run)
	{
		Display::clear(Color(0.9f, 0.3f, 0.4f));

		tex->draw(100, 100);

		Display::render();

		Display::swap();

		mh.process_events();
	}

	pix_shutdown();

	return 0;
}

