#include "pix5.h"
#include <math.h>


struct MyTile
{
	TILE_BASE
};

TileMap<MyTile> *tm;


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
	}

	void on_key_up(Key& key)
	{
	}

	void on_text(const Str& text)
	{
	}

	void on_window_resize(int width, int height)
	{
		tm->set_rect(Rect(0, 0, width, height));
	}

	void on_quit()
	{
		run = false;
	}

	void on_joy_axis(int id, int axis, float val)
	{
		printf("AXIS %i:%i  :  %f\n", id, axis, val);
	}

	void on_joy_button(int id, int but, bool val)
	{
		printf("BUT  %i:%i  :  %s\n", id, but, val ? "Down" : "Up");
	}

	void on_joy_hat(int id, int hat, HatDirection::Type val)
	{
		printf("HAT  %i:%i  :  %i\n", id, hat, (int)val);
	}
};




void *t(void *p)
{

	return NULL;
}


int main(int argc, char **argv)
{
	pthread_t th;
	pthread_create(&th, NULL, t, NULL);

	try
	{
		pix_init("Test");

		Display::set_mode(VideoMode::resizable(
			1366, 768, false
		));


		tm = new TileMap<MyTile>(256, 256, 32, 32);
		tm->add_layer_tiles(Cache::texture("data/ts0.png"));


		for(int y = 0; y<256; y++)
			for(int x = 0; x<256; x++)
			{
				MyTile *t = tm->get_tile(0, x, y);
				t->tile = rand()%171;
			}


		Texture *tex = Texture::create(502, 302);
		tex->rect(0, 0, 501, 301, Color(1,1,1));

		MyEventHandler eh;

		int bx = 0;
		int by = 0;


		uint32_t vt = tm->add_virt_tile(0, 18, 300);
		tm->add_virt_tile(vt, 19, 300);
		tm->add_virt_tile(vt, 20, 300);


		tm->get_tile(0, 4, 3)->tile = vt;


		tm->set_rect(Rect(300, 250, 500, 300));
		//tm->set_rect(Rect(0, 0, 1246, 1059));

		Texture *at = Cache::texture("data/sprites.png");

		Anim *a = new Anim();
		a->add_frame("walk_down", at, 10, 0, 0, 32*3, 55, 3, 1);
		a->add_frame("walk_left", at, 10, 0, 55, 32*3, 55, 3, 1);
		a->add_frame("walk_right", at, 10, 0, 110, 32*3, 55, 3, 1);
		a->add_frame("walk_up", at, 10, 0, 165, 32*3, 55, 3, 1);

		Anim *a2 = new Anim();
		a2->add_frame("walk_down", at, 10, 96, 0, 32*3, 55, 3, 1);
		a2->add_frame("walk_left", at, 10, 96, 55, 32*3, 55, 3, 1);
		a2->add_frame("walk_right", at, 10, 96, 110, 32*3, 55, 3, 1);
		a2->add_frame("walk_up", at, 10, 96, 165, 32*3, 55, 3, 1);


		tm->add_layer_sprites();

		for(int c = 0; c<10000; c++)
		{
			MapSprite *ms = tm->new_sprite(1, a2, rand()%40, rand()%25);
			ms->set_origin(16, 55-16);
		}

		int sx = 700;
		int sy = 400;

		SpriteSet ss;
		Sprite *s = ss.new_sprite(a, 0, sx, sy);

		while(run)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));
			//Display::clear();

			//Display::test_draw(299, 249, tex);

			tm->set_pos(bx, by);
			tm->draw();

			ss.draw();

			try
			{
				Joystick joy = Input::get_joystick(1);

				if(Input::get_keyboard()[KEY_LEFT] || joy.hats[0]->left())
				{
					bx-=2;
					s->change_set("walk_left");
				}

				else if(Input::get_keyboard()[KEY_RIGHT] || joy.hats[0]->right())
				{
					bx+=2;
					s->change_set("walk_right");
				}

				else if(Input::get_keyboard()[KEY_UP] || joy.hats[0]->up())
				{
					by-=2;
					s->change_set("walk_up");
				}

				else if(Input::get_keyboard()[KEY_DOWN] || joy.hats[0]->down())
				{
					by+=2;
					s->change_set("walk_down");
				}
			}
			catch(Error)
			{
			}

			Display::swap();

			eh.process_events();
		}

		delete tm;

		pix_shutdown();
	}
	catch(Error e)
	{
		printf("Error: %s\n", e.format().ptr());
	}

	return 0;
}


