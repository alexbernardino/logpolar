#ifndef CIMG_H
#define CIMG_H

#define COLOR_MODE_INTERLEAVED 0
#define COLOR_MODE_PLANAR 1

class CImg
{
	int width;
	int height;
	int stride;
	int depth;
	int color_channels;
	int color_mode;
	void *data;
};

#endif //CIMG_H