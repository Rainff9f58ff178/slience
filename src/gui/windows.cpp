
#include "gui/windows.h"






Windows::Windows(Widget* parent,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		uint8_t r, uint8_t g, uint8_t b):CompositeWidget(parent,x,y,w,h,r,g,b){
	this->draging=false;
}
Windows::~Windows();



void Windows::onMouseMove(int32_t old_x,int32_t old_y,int32_t new_x,int32_t new_y){
	if(draging){
		this->x+=new_x-old_x;
		this->y+=new_y-old_y;
	}
	CompositeWidget::onMouseMove(old_x,old_y,new_x,new_y);
}
void Windows::onMouseDown(int32_t x,int32_t y,uint8_t button){
	draging=(button==1);
	CompositeWidget::onMouseDown(x,y,button);
}
void Windows::onMouseUp(int32_t x,int32_t y,uint8_t button){
	draging=false;
	CompositeWidget::onMouseUp(x,y,button);
}
