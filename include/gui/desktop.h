


#ifndef __GUI_DESKTOP_H
#define __GUI_DESKTOP_H

#include "gui/widget.h"
#include "driver/mouse.h"
class Desktop:public CompositeWidget,public MouseEventHandler{
protected:
	uint32_t mouse_x;
	uint32_t mouse_y;
public:

	Desktop(uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b);
	~Desktop();

	void Draw(GraphicsContext* gc);//CompositeWidget wont draw the mouse,wo we nedd add the function

	void OnMouseMove(int x,int y);
	void OnMouseDown(uint8_t button);
	void OnMouseUp(uint8_t button);



};


#endif