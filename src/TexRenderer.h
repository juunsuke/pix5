#pragma once



class TexRenderer
{
	List<struct TRTex*> _tex;
	// List of queued textures

	VertexArray _va;
	// Our vertex array

public:

	TexRenderer();
	~TexRenderer();

	void clear();
	// Clear the queued list of textures

	void add(Texture *tex, int x, int y, const Color& col = Color(1,1,1,1), int w = -1, int h = -1, float u1 = 0, float v1 = 0, float u2 = 1.0f, float v2 = 1.0f);
	// Add a texture to the queue
	// If 'w' or 'h' are -1, the size are taken from the texture

	void add(Anim *anim, int x, int y, const Color& col = Color(1,1,1,1), int w = -1, int h = -1, bool advance = true);
	// Add the current frame of the given anim
	// If 'w' or 'h' are -1, the size are taken from the frame
	// 'advance' is passed to Anim::get_frame()

	void draw(int ox = 0, int oy = 0);
	// Draw all the queued textures
	// ox,oy are the global offset to add to each texture's destination coordinates
	// When this is done, there will be no VertexArray, Shader or Texture bound
	// The 2D camera will be set before drawing
	// After rendering, the queue will be cleared
};

