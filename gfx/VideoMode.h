#pragma once


class VideoMode
{
	void copy(const VideoMode& o)
	{
		type = o.type;
		width = o.width;
		height = o.height;
		vsync = o.vsync;
		x = o.x;
		y = o.y;
		renderer = o.renderer;
	}

public:

	VideoModeType::Type type;
	// Type

	int width, height;
	// Resolution

	bool vsync;
	// Wether to use vsync

	int x, y;
	// Used to control the window position for windowed and resizable modes

	RendererType::Type renderer;
	// Renderer type to use


	VideoMode()
	{
		clear();
	}

	VideoMode(const VideoMode& o)
	{
		// Copy constructor
		copy(o);
	}

	VideoMode(VideoModeType::Type type, int width, int height, bool vsync = true, int x = 0x2FFF0000, int y = 0x2FFF0000)
	{
		this->type = type;
		this->width = width;
		this->height = height;
		this->vsync = vsync;
		this->x = x;
		this->y = y;
		this->renderer = RendererType::Shader;
	}

	~VideoMode()
	{
	}


	VideoMode& operator=(const VideoMode& o)
	{
		// Affectation
		copy(o);
		return *this;
	}



	void clear()
	{
		// Default empty mode
		type = VideoModeType::None;
		width = 0;
		height = 0;
		vsync = true;
		x = 0x2FFF0000;
		y = 0x2FFF0000;
		renderer = RendererType::Shader;
	}




	//
	// Static functions to create video modes easily
	//

	static VideoMode window(int width = 1366, int height =768, bool vsync = true, int x = 0x2FFF0000, int y = 0x2FFF0000)
	{
		return VideoMode(VideoModeType::Window, width, height, vsync, x, y);
	}

	static VideoMode resizable(int width = 1366, int height =768, bool vsync = true, int x = 0x2FFF0000, int y = 0x2FFF0000)
	{
		return VideoMode(VideoModeType::Resizable, width, height, vsync, x, y);
	}

	static VideoMode desktop(bool vsync = true)
	{
		return VideoMode(VideoModeType::Desktop, 0, 0, vsync);
	}

	static VideoMode fullscreen(int width, int height, bool vsync = true)
	{
		return VideoMode(VideoModeType::Fullscreen, width, height, vsync);
	}

};


