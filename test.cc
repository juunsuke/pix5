#include "pix5.h"


static bool _run = true;


UdpEndPoint *cli, *srv;


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
			NetMessage *msg = new NetMessage();
			msg->add_i32(748);
			msg->add_str("Proutentakendok");

			for(int c = 0; c<12000; c++)
				msg->add_i64(c*c);

			msg->add_str("Shtre");

			cli->send_message(msg);
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




	cli = new UdpEndPoint();
	cli->connect(SockAddr(Net::resolve("localhost"), 6464));



	srv = new UdpEndPoint();
	srv->listen(SockAddr(0, 6464));


	List<MountEntry*> m;
	IO::get_mounts(m);

	for(int c = 0; c<m.size(); c++)
		printf("[%i] '%s:' -> %s\n", c, m[c]->mount_point.ptr(), m[c]->desc.ptr());

	m.clear_del();



	List<FileEntry*> fe;

	IO::get_files("home:/", fe);

	for(int c = 0; c<fe.size(); c++)
		printf("[%3i]  %c  %10li  %s\n", c, fe[c]->dir ? '*':' ', fe[c]->size, fe[c]->path.full().ptr());

	fe.clear_del();


	while(_run)
	{
		cli->handle();
		srv->handle();

		if(cli->state()==EndPointState::Error)
			printf("Client is error: %s\n", cli->error().ptr());

		if(srv->state()==EndPointState::Error)
			printf("Server is error: %s\n", srv->error().ptr());

		if(srv->state()==EndPointState::Connected)
		{
			NetMessage *msg = srv->recv_message();
			if(msg)
			{
				printf("Received %i bytes message\n", msg->size());

				printf("i32: %i\n", msg->get_i32());
				printf("str: '%s'\n", msg->get_str().ptr());

				for(int c = 0; c<12000; c++)
					//printf("%4i: %li\n", c, msg->get_i64());
					msg->get_i64();
				
				printf("str: '%s'\n", msg->get_str().ptr());

				delete msg;
			}
		}

		Display::clear(Color(0.9f, 0.3f, 0.4f));

		tex->draw(100, 100);

		Display::render();

		Display::swap();

		mh.process_events();
	}

	delete cli;
	delete srv;

	pix_shutdown();

	return 0;
}

