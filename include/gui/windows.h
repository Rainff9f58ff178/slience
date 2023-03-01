


#ifndef __GUI_WINDOWS_H
#define __GUI_WINDOWS_H
#include "common/types.h"
#include "gui/widget.h"
class Windows:public CompositeWidget{
protected:
	bool draging;
public:
	Windows(Widget* parent,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		uint8_t r, uint8_t g, uint8_t b);
	~Windows();



	void onMouseMove(int32_t old_x,int32_t old_y,int32_t new_x,int32_t new_y);
	void onMouseDown(int32_t x,int32_t y,uint8_t button);
	void onMouseUp(int32_t x,int32_t y,uint8_t button);

};




#endif