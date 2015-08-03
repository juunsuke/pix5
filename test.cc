#include "pix5.h"


static bool _run = true;


UdpSocket *sok;


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
			static int counter = 0;
			Str s = Str::build("Oink oink %i\n", counter++);
			sok->send(SockAddr(Net::resolve("localhost"), 9999), s.ptr(), s.len());
		}
	}

};


int main(int argc, char **argv)
{
	pix_init("Test");

	Display::set_mode(VideoMode::resizable());

	MyHandler mh;

	Texture *tex = Texture::cache("src/data/konata.png");

	SockAddr sa(Net::resolve("localhost"), 9999);


	Str str = "Proute\r\n";

	sok = new UdpSocket();
	if(sok->send(sa, str.ptr(), str.len()))
		printf("Success\n");
	else
		printf("Failure\n");

	while(_run)
	{
		char buf[512];
		int r = sok->recv(sa, buf);
		if(r)
		{
			buf[r] = 0;
			printf("Received %i bytes from %s: %s\n", r, sa.format_ip_port().ptr(), buf);
		}

		Display::clear(Color(0.9f, 0.3f, 0.4f));

		tex->draw(100, 10);

		Display::render();

		Display::swap();

		mh.process_events();
	}

	delete sok;

	pix_shutdown();

	return 0;
}

