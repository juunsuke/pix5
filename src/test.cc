#include "pix5.h"
#include <math.h>


Sprite *s;


class MyEventHandler: public EventHandler
{
public:

	void on_mouse_wheel_down(Mouse& mouse)
	{
		s->set_scale(s->hscale()*0.9f);
	}

	void on_mouse_wheel_up(Mouse& mouse)
	{
		s->set_scale(s->hscale()*1.1f);
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




int main(int argc, char **argv)
{
	try
	{
		pix_init("Test");

		Display::set_mode(VideoMode::resizable());
		//Display::set_mode(VideoMode::resizable(1366, 768, false));

		MyEventHandler eh;

		SpriteSet *ss = new SpriteSet();

		Texture *tex = Texture::load("data/konata.png");
		tex->set_filter(TextureFilter::Linear, TextureFilter::Linear);

		s = ss->new_sprite(tex);
		s->set_origin(tex->width()/2, tex->height()/2);

		float a = 0;

		for(;;)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));
			//Display::clear();

			int mx = Input::get_mouse().x;
			int my = Input::get_mouse().y;

			if(mx!=s->x() || my!=s->y())
				s->set_pos(mx, my);

			a += 1.0f;
			s->set_angle(a);

			ss->draw();

			Display::swap();

			eh.process_events();
		}

		delete ss;

		pix_shutdown();
	}
	catch(Error e)
	{
		printf("Error: %s\n", e.format().ptr());
	}

	return 0;
}


