

#ifndef __GUI_COLOR_H
#define __GUI_COLOR_H

#include "common/types.h"
class Color{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color(const Color& color);
	Color();
	Color(uint8_t r,uint8_t g,uint8_t b);
	~Color();
};

#endif