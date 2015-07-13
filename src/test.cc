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



//
// Custom Vertex
// Each square texture has 4 vertex
typedef struct CustomVertex
{
	float x, y;
	// Position of the vertex on screen, in 2D coordinates

	float u, v;
	// U/V coordinates - Texture coordinates

	uint32_t col;
	// Color for that vertex
} CustomVertex;



int main(int argc, char **argv)
{
	try
	{
		pix_init("Test");

		SoundClip *sc = SoundClip::load("data/FindYou.ogg", "music");
		sc2 = SoundClip::load("data/Heal8-Bit.ogg", "fx");

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
		
		tex = Cache::texture("data/konata.png");
		tex->set_filter(TextureFilter::Linear, TextureFilter::Linear);

		s = ss->new_sprite(tex);
		s->set_origin(tex->width()/2, tex->height()/2);
		s->set_z(500);
		s->set_scale(2);


		float a = 0;



		////////////////
		/////////////


		// Create a vertex definition
		VertexDef vd;
		
		// Add the position component of the vertex
		// Position has 2 floats, so call float2()
		// The shader variable name for position is "pos"
		// The position starts at byte 0 in the vertex struct
		vd.add(VertexComp::float2("pos", 0));

		// Add the texture coordinate component of the vertex
		// It also has 2 floats, so float2()
		// The variable name in the shader is "texcoord"
		// In the struct, it comes after "x" and "y", each is a 4-byte float
		// So the starting position is at the 8th byte
		vd.add(VertexComp::float2("texcoord", 8));

		// Add the color component
		// It has 4 unsigned bytes, so ubyte4()
		// The values are normalized, so true
		// The variable name in the shader is "col"
		// In the struct, it comes after "x", "y", "u", "v", each is 4-byte float
		// So starting position is at the 16th byte
		vd.add(VertexComp::ubyte4("col", true, 16));

		// Create the Vertex Array (which is a collection of Vertex Buffers)
		VertexArray *va = new VertexArray();

		// Add a new stream to it (1 stream = 1 sub-vertex-buffer)
		// For simplicity, 1 is enough
		// It needs the definition, and the size of the vertex struct
		va->add_stream(vd, sizeof(CustomVertex));



		// Each texture requres 4 vertex to be draw
		// Top-Left corner
		// Top-Right corner
		// Bottom-Right corner
		// Bottom-Left corner

		// So lock the 4 first vertex of stream 0
		// lock(stream, first, count)
		CustomVertex *v = (CustomVertex*)va->lock(0, 0, 4);

		// To lock the 4 next vertex:
		// va->lock(0, 4, 4);
		// To lock the 16 first vertex, that can draw 4 textures:
		// va->lock(0, 0, 16)

		// Fill in the vertex data

		// Top-left corner
		v[0].x = 10;
		v[0].y = 10;
		v[0].u = 0;
		v[0].v = 0;
		v[0].col = 0xFFFFFFFF;

		// Top-right corner
		v[1].x = 300;
		v[1].y = 10;
		v[1].u = 1;
		v[1].v = 0;
		v[1].col = 0xFFFFFFFF;

		// Bottom-right corner
		v[2].x = 300;
		v[2].y = 500;
		v[2].u = 1;
		v[2].v = 1;
		v[2].col = 0xFFFFFFFF;

		// Bottom-left corner
		v[3].x = 10;
		v[3].y = 500;
		v[3].u = 0;
		v[3].v = 1;
		v[3].col = 0xFFFFFFFF;


		// Load the sprite shader
		Shader *shad = Shader::load("TestShader", "data/sprite.vs", "data/sprite.fs");

		// Compile it
		shad->compile();

		/////////////////
		////////////////

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

			//ss->draw();


			////////////////////
			///////////////////

			// Set a 2D camera
			Display::get_2d_camera().set();

			// Bind the shader (tell OpenGL to use it)
			shad->bind();
		
			// Set the shader's matrix
			// Don't ask, just do it
			shad->set_uniform("mat", Matrix::identity());


			// Bind the vertex array (tell OpenGL to use it)
			// It needs to know the shader you're using
			va->bind(shad);

			// Bind the texture (tell OpenGL to use it)
			// Put 0 and don't ask
			tex->bind(0);

			// Draw 4 vertex, starting from the 1st one
			// draw() will use the bound shader, vertex array and texture to draw
			Display::draw(0, 4);

			//////////////////
			//////////////////


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


