#pragma once


class VideoData
{
	VideoData *_prev, *_next;
	// Used for the doubly linked list

	friend void vd_init();
	friend void vd_done();

public:

	VideoData();
	virtual ~VideoData();


	virtual void release_video_data() {};
	// Called before shutting down OpenGL or switching video modes


	static void release_all();
	// Release all video data
};

