#include "pix5.h"
#include <math.h>


Sprite *s;
SoundClip *sc2;

class MyEventHandler: public EventHandler
{
public:

	void on_mouse_wheel_down(Mouse& mouse)
	{
		//s->set_scale(s->hscale()*0.9f);
		int nz = s->z()-1;
		s->set_z(nz);
		Log::debug("Z: %i", nz);
	}

	void on_mouse_wheel_up(Mouse& mouse)
	{
		//s->set_scale(s->hscale()*1.1f);
		int nz = s->z()+1;
		s->set_z(nz);
		Log::debug("Z: %i", nz);
	}

	void on_mouse_move(Mouse& mouse)
	{
		//printf("%i, %i\n", mouse.x, mouse.y);
	}

	void on_key_down(Key& key)
	{
		if(key.code==KEY_SPACE)
			Audio::play(sc2);
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

		SoundClip *sc = SoundClip::load("data/FindYou.ogg");
		sc2 = SoundClip::load("data/Heal8-Bit.ogg");

		Audio::play(sc, true);

		Display::set_mode(VideoMode::resizable());
		//Display::set_mode(VideoMode::resizable(1366, 768, false));

		MyEventHandler eh;

		SpriteSet *ss = new SpriteSet();

		Font *fnt = new Font("data/LiberationSans-Regular.ttf", 12);

		Texture *tex;

		for(int c = 0; c<1000; c++)
		{
			int n = rand()%1000;

			tex = Texture::create(100, 20, false);
			tex->clear(0xFF000000 | (rand()*rand()));

			tex->print(fnt, 5, 3, Color(0,0,0), Str::build("%i", n));

			s = ss->new_sprite(tex, n, rand()%1200, rand()%1000);
		}
		
		tex = Texture::load("data/konata.png");
		tex->set_filter(TextureFilter::Linear, TextureFilter::Linear);

		s = ss->new_sprite(tex);
		s->set_origin(tex->width()/2, tex->height()/2);
		s->set_z(500);
		s->set_scale(2);


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


