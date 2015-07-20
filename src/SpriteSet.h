#pragma once


class SpriteSet
{
	VertexArray _va;
	// The set's vertex array

	SortedList<Sprite*> _vis_sprites;
	// List of all visible sprites, sorted by their Z value

	Sprite *_hid_first, *_hid_last;
	// First/last hidden sprite, for the doubly-linked list

	bool _va_dirty;
	// Wether the vertex array should be rebuild

	int _max_size;
	// Max number of visible sprites, used to handle the vertex buffers properly

	
	void show_sprite(Sprite *s);
	// Show a hidden sprite

	void hide_sprite(Sprite *s);
	// Hide a visible sprite

	void reorder_sprite(Sprite *s, int new_z);
	// Change a sprite's Z value, changing its ordering if visible

	void add_hidden(Sprite *s);
	void del_hidden(Sprite *s);
	// Add/remove a sprite to the hidden doubly-linked-list

	void add_visible(Sprite *s);
	void del_visible(Sprite *s);
	// Add/remove a sprite from the visible list

	void set_sprite_vertex(struct SpriteVertex *v, Sprite *s);
	// Set a sprite's vertex data

	void check_max();
	// Check/handle the max size

	void set_pos_data(int first, int num);
	// Fill in part of the position vertex buffer
	
	void del_sprite(Sprite *s);
	// Delete a sprite that is part of this SpriteSet

	friend class Sprite;

public:

	SpriteSet();
	~SpriteSet();


	void draw();
	// Render all the sprites in the set
	// Any sprites bound to an animation will advance the animations of all visible sprites

	Sprite *new_sprite(Texture *tex, int z=0, int x=0, int y=0, int ox=0, int oy=0, bool vis=true, float u1=0, float v1=0, float u2=1, float v2=1);
	Sprite *new_sprite(Anim *anim, int z=0, int x=0, int y=0, int ox=0, int oy=0, bool vis=true);
	// Create a new sprite in the set
};

