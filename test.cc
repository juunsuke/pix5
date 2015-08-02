#include "pix5.h"


class MyHandler: public GfxEventHandler
{
public:

};


int main(int argc, char **argv)
{
	pix_init("Test");

	Display::set_mode(VideoMode::resizable());

	MyHandler mh;

	Texture *tex = Texture::cache("src/data/konata.png");

	TcpSocket *s;
	s = new TcpSocket();

	SockAddr sa(Net::resolve("www.google.ca"), 80);

	s->create();
	s->connect(sa);

	for(;;)
	{
		Display::clear(Color(0.9f, 0.3f, 0.4f));

		tex->draw(100, 10);

		Display::render();

		Display::swap();

		mh.process_events();
	}

	pix_shutdown();

	return 0;
}

