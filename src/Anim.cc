#include "pix5.h"

namespace PIX {




Anim::Anim()
{
	_cur_set = NULL;
	_cur_frame = 0;
	_frames = 0;
}

Anim::~Anim()
{
	_sets.clear_del();
}

AnimSet *Anim::get_set(const Str& name, bool add)
{
	// Find the set
	for(int c = 0; c<_sets.size(); c++)
		if(_sets[c]->_name==name)
			return _sets[c];

	// Doesn't exist, create it  ?
	if(!add)
		// No
		return NULL;

	// Yes
	AnimSet *set = new AnimSet(name);
	_sets.add(set);

	// If it's the first set, make it active
	if(!_cur_set)
	{
		_cur_set = set;
		_cur_frame = 0;
		_frames = 0;
	}

	return set;
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames)
{
	return add_frame(set_name, tex, frames, 0, 0, tex->width(), tex->height());
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames, int x, int y, int w, int h)
{
	ASSERT(w>0 && h>0, "Anim::add_frame(): Invalid width/height")

	// Get/create the set
	AnimSet *set = get_set(set_name, true);

	// Add the frame to it
	AnimFrame *frame = new AnimFrame();
	frame->tex = tex;
	frame->frames = frames;

	frame->w = w;
	frame->h = h;

	float uf = 1.0f / (float)tex->width();
	float vf = 1.0f / (float)tex->height();

	frame->u1 = (float)x*uf;
	frame->v1 = (float)y*vf;
	frame->u2 = (float)(x+w)*uf;
	frame->v2 = (float)(y+h)*vf;

	// Add it
	return set->_frames.add(frame);
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames, int hdiv, int vdiv)
{
	return add_frame(set_name, tex, frames, 0, 0, tex->width(), tex->height(), hdiv, vdiv);
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames, int x, int y, int w, int h, int hdiv, int vdiv)
{
	ASSERT(hdiv>0 && vdiv>0, "Anim::add_frame(): Invalid h/v sub-division factor")
	ASSERT((w%hdiv)==0, "Anim::add_frame(): Width is not divisible by hdiv")
	ASSERT((h%vdiv)==0, "Anim::add_frame(): Height is not divisible by vdiv")

	// Add all the sub-frames
	int fw = w/hdiv;
	int fh = h/vdiv;

	for(int cy = 0; cy<vdiv; cy++)
		for(int cx = 0; cx<hdiv; cx++)
			add_frame(set_name, tex, frames, x+cx*fw, y+cy*fw, fw, fh);

	// Return the index of the first frame
	return get_set(set_name, false)->_frames.size()-(fw*fh);
}

void Anim::set_set(const Str& name, int frame)
{
	// Change the current set
	AnimSet *set = get_set(name, false);
	if(!set)
		E::BadAnimSet("Anim::set_set(): Unknown set: '%s'", name.ptr());

	// Adjust frame properly
	if(frame<0 || frame>=set->_frames.size())
		frame = 0;

	// Make it active
	_cur_set = set;
	_cur_frame = frame;
	_frames = 0;
}
	
void Anim::change_set(const Str& name, int frame)
{
	// Do nothing if the current set is the given one
	if(_cur_set && _cur_set->_name==name)
		return;

	// Change the set
	set_set(name, frame);
}

AnimFrame *Anim::get_frame(bool advance)
{
	if(!_cur_set)
		E::BadAnimSet("Anim::get_frame(): No defined set");

	// Get the current frame
	AnimFrame *frm = _cur_set->_frames[_cur_frame];

	// Advance if needed
	if(advance)
	{
		// Advance the counter by 1 frame
		_frames++;

		// Check if it reached the current frame's limit
		if(_frames>=frm->frames)
		{
			// Yes, advance to the next frame
			_cur_frame++;
			if(_cur_frame>=_cur_set->_frames.size())
				_cur_frame = 0;

			_frames = 0;
		}
	}

	return frm;
}



}
