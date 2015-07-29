#include "pix5.h"
#include "data.h"


struct MyTile
{
	TILE_BASE
};

TileMap<MyTile> *tm;


bool run = true;
Texture *at;

Lua lua;


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
		if(key==KEY_Q)
			Input::show_cursor();
		if(key==KEY_W)
			Input::hide_cursor();
		if(key==KEY_E)
			Input::set_cursor(at->sub(0, 0, 32, 55));
		if(key==KEY_R)
		{
			Texture *t = Texture::create(20, 12);
			t->rect(0, 0, 18, 6, Color(0, 0, 0, 1));
			t->rect_fill(1, 1, 17, 5, Color(1, 1, 1, 1));

			Input::set_cursor(t, 0, 0, true);
		}

		if(key.code>=KEY_1 && key.code<=KEY_ESCAPE)
		{
			Input::set_cursor((SystemCursor::Type)(key.code-KEY_1));
		}
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
		//printf("AXIS %i:%i  :  %f\n", id, axis, val);
	}

	void on_joy_button(int id, int but, bool val)
	{
		//printf("BUT  %i:%i  :  %s\n", id, but, val ? "Down" : "Up");
	}

	void on_joy_hat(int id, int hat, HatDirection::Type val)
	{
		//printf("HAT  %i:%i  :  %i\n", id, hat, (int)val);
	}

	void on_action(const Str& action)
	{
		Log::log("Action: '%s'", action.ptr());
	}

	void on_joy_connect(int id)
	{
		Input::map_joystick("proute", id);
	}
};



void test()
{
}


int main(int argc, char **argv)
{
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


		Texture *tex = Texture::create(502, 402);
		tex->rect(0, 0, 501, 401, Color(1,1,1));

		MyEventHandler eh;

		int bx = 0;
		int by = 0;


		uint32_t vt = tm->add_virt_tile(0, 18, 300);
		tm->add_virt_tile(vt, 19, 300);
		tm->add_virt_tile(vt, 20, 300);


		tm->get_tile(0, 4, 3)->tile = vt;


		tm->set_rect(Rect(300, 250, 500, 300));
		//tm->set_rect(Rect(0, 0, 1246, 1059));

		at = Cache::texture("data/sprites.png");


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

		SpriteSet ss;
		Sprite *s = ss.new_sprite(a, 0, 700, 400);


		Action::map_key("main", "jump", KEY_SPACE);
		Action::map_mouse_button("main", "jump", 33);

		Action::map_joy_button("main", "move_down", "proute", 0);
		Action::map_joy_button("main", "move_up", "proute", 3);

		Action::map_key("main", "move_left", KEY_LEFT);
		Action::map_key("main", "move_right", KEY_RIGHT);
		Action::map_key("main", "move_up", KEY_UP);
		Action::map_key("main", "move_down", KEY_DOWN);
		
		Action::map_mouse_button("main", "move_left", 1);
		Action::map_mouse_button("main", "move_right", 3);

		Action::map_joy_hat("main", "move_left", "proute", 0, HatDirection::Left);
		Action::map_joy_hat("main", "move_right", "proute", 0, HatDirection::Right);
		Action::map_joy_hat("main", "move_up", "proute", 0, HatDirection::Up);
		Action::map_joy_hat("main", "move_down", "proute", 0, HatDirection::Down);

		Action::enable("main");


		TextFormat tf;

		tf.set_align(TextAlign::Center);
		tf.set_valign(VertTextAlign::Middle);
		tf.add_text("Elfrea is a nigger\n\n");

		tf.set_align(TextAlign::Justify);
		int z[6] = { -1, -1, -1, -1, -1, -1};
		z[0] = tf.add_text("Résolu - In the English language, nigger is an ");
		
		tf.set_color(Color(0, 1, 0));
		tf.set_font(Cache::font(data_LiberationSans_Regular_ttf, data_LiberationSans_Regular_ttf_len, 18));
		z[1] = tf.add_text("Ethnic Slur");

		tf.set_font();
		tf.set_color(Color(1, 1, 1));
		tf.set_bg_color(Color(0, 0, 1));

		z[2] = tf.add_text(" usually directed at black people. The word originated as a neutral term referring to people with black skin,[1] as a variation of the Spanish/Portuguese noun negro, a descendant of the Latin adjective niger (\"black\").[2]\n\n");

		tf.set_align(TextAlign::Right);
		tf.set_valign();
		
		z[3] = tf.add_text("It was often used disparagingly, and ");

		tf.add_tex(Cache::texture("data/sprites.png"), Rect(0, 0, 32, 55));

		tf.set_valign(VertTextAlign::Middle);

		z[5] = tf.add_text(" by the mid-20th century, particularly in the United States, its usage became unambiguously pejorative, a racist insult. Accordingly, it began to disappear from popular culture, and its continued inclusion in classic works of literature has sparked controversy.\n");

		tf.add_newline();
		tf.set_align(TextAlign::Left);
		tf.add_text("Proute");
		tf.set_align(TextAlign::Right);
		tf.add_text("Oink");

		TextFormat tf2 = tf;

		tf2.calc(tex->width());
		tex->rect(Rect(0, 0, tf2.get_width(), tf2.get_height()), Color(1, 1, 0));
		tf2.render(tex);

		for(int zz = 0; zz<6; zz++)
		{
			if(z[zz]!=-1)
			{
				/*List<Rect> lr = tf2.get_pos(z[zz]);
				for(int c = 0; c<lr.size(); c++)
					tex->rect(lr[c], Color(1, 0, 0));
				lr.clear_nodel();*/
			}
		}

		tex->line(10, 10, 500, 300, 3, Color(0.9f, 0.4f, 0.3f));
		tex->line(10, 50, 500, 340, 3, Color(0.9f, 0.4f, 0.3f, 0.5f));

		printf("\n\n");

		lua.load("test.lua");
		LuaData *ld = lua.get_data("z");
		ld->log();


		printf("\n\n");

		Str str = "Résolu";
		printf("%i   %i    %i\n", str.len(), str.get_utf8_len(), str.get_utf8_start(2));

		while(run)
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));
			//Display::clear();

			tm->set_pos(bx, by);
			tm->draw();

			ss.draw();
			
			Display::test_draw(299, 249, tex);

			if(Action::is_down("move_left"))
			{
				bx-=2;
				s->change_set("walk_left");
			}

			if(Action::is_down("move_right"))
			{
				bx+=2;
				s->change_set("walk_right");
			}

			if(Action::is_down("move_up"))
			{
				by-=2;
				s->change_set("walk_up");
			}

			if(Action::is_down("move_down"))
			{
				by+=2;
				s->change_set("walk_down");
			}

			Display::swap();

			eh.process_events();
		}

		delete a;
		delete a2;
		delete tm;
		delete tex;

		pix_shutdown();
	}
	catch(Error e)
	{
		printf("Error: %s\n", e.format().ptr());
	}

	return 0;
}


