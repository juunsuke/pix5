#include "pix5.h"


static bool _run = true;


SoundClip *sc, *sc2;



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
			Audio::play(sc, -1, false, 1, 0, 0.5f);
		if(key==KEY_LEFT)
			Audio::play(sc, -1, false, 1.0f, -1.0f);
		if(key==KEY_RIGHT)
			Audio::play(sc, -1, false, 1.0f, 1.0f);
		if(key==KEY_Q)
			Audio::set_pos(0, Audio::get_pos(0)-0.1f);
		if(key==KEY_W)
			Audio::set_pos(0, Audio::get_pos(0)+0.1f);
		if(key==KEY_P)
			Audio::pause(0);
		if(key==KEY_O)
			Audio::resume(0);
		if(key==KEY_X)
			Audio::stop(0, 5);
		if(key==KEY_UP)
			Audio::set_volume("music", Audio::get_volume("music")+0.1f);
		if(key==KEY_DOWN)
			Audio::set_volume("music", Audio::get_volume("music")-0.1f);
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


	sc = SoundClip::load("Heal8-Bit.ogg");

	sc2 = SoundClip::load("FindYou.ogg", "music");

	Audio::play(sc2, -1, true);


	while(_run)
	{
		Display::clear(Color(0.9f, 0.3f, 0.4f));

		tex->draw(100, 100);

		Display::render();

		Display::swap();

		mh.process_events();
	}

	delete sc;
	delete sc2;

	pix_shutdown();

	return 0;
}

