#include "pix5gfx.h"


static VideoData *_first = NULL;
static VideoData *_last = NULL;
// First and last entries for the double linked list


void vd_init()
{
	// Create the initial elements
	_first = new VideoData();
	_last = new VideoData();

	_first->_next = _last;
	_first->_prev = NULL;

	_last->_next = NULL;
	_last->_prev = _first;
}

void vd_done()
{
	// Delete the first and last
	VideoData *f = _first;
	VideoData *l = _last;

	_first = NULL;
	_last = NULL;

	if(f)
		delete f;

	if(l)
		delete l;
}


VideoData::VideoData()
{
	// Link, unless the head and tail aren't yet created
	if(!_last)
		return;

	_prev = _first;
	_next = _first->_next;

	_prev->_next = this;
	_next->_prev = this;
}

VideoData::~VideoData()
{
	// Unlink, unless there are no heads/tails
	if(!_last)
		return;

	_prev->_next = _next;
	_next->_prev = _prev;
}

void VideoData::release_all()
{
	// Release all the video data
	for(VideoData *run = _first->_next; run!=_last; run = run->_next)
		run->release_video_data();
}

