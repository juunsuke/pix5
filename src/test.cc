#include "pix5.h"
#include <math.h>


struct MyTile
{
	TILE_BASE
};

TileMap<MyTile> *tm;



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
};



int main(int argc, char **argv)
{
	try
	{
		pix_init("Test");

		Display::set_mode(VideoMode::resizable(
			//1366, 768, false
		));


		tm = new TileMap<MyTile>(256, 256, 32, 32);
		tm->add_layer_tiles(Cache::texture("data/ts0.png"));


		for(int y = 0; y<256; y++)
			for(int x = 0; x<256; x++)
			{
				MyTile *t = tm->get_tile(0, x, y, true);
				t->tile = rand()%171;
			}


		Texture *tex = Texture::create(802, 502);
		tex->rect(0, 0, 801, 501, Color(1,1,1));

		MyEventHandler eh;

		int bx = 0;
		int by = 0;

		for(;;)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));
			//Display::clear();

			tm->draw(Rect(100, 50, 800, 500), bx, by);

			Display::test_draw(99, 49, tex);

			if(Input::get_keyboard()[KEY_LEFT])
				bx-=2;

			if(Input::get_keyboard()[KEY_RIGHT])
				bx+=2;

			if(Input::get_keyboard()[KEY_UP])
				by-=2;

			if(Input::get_keyboard()[KEY_DOWN])
				by+=2;

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


