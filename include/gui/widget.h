

#ifndef __GUI_WIDGET_H
#define __GUI_WIDGET_H

#include "gui/color.h"
#include "common/graphicscontext.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
class Widget:public KeyboardEventHandler{
protected:
	Widget* parent;
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;

	uint8_t r;
	uint8_t g;
	uint8_t b;


	bool focuessable;
public:
	Widget(Widget* parent,
	int32_t x,
	int32_t y,
	int32_t w,
	int32_t h,
	uint8_t r,uint8_t g,uint8_t b);

	~Widget();
	virtual void getFocus(Widget* widget);
	virtual void modelToScreen(int32_t& x,int32_t& y);
 

	virtual void Draw(GraphicsContext* gc);
	virtual void onMouseMove(int32_t old_x,int32_t old_y,int32_t new_x,int32_t new_y);
	virtual void onMouseUp(int32_t x,int32_t y,uint8_t button);
	virtual void onMouseDown(int32_t x,int32_t y,uint8_t button);

	virtual void OnKeyDown(char str);                    
	virtual void OnKeyUp(char str);

	virtual bool containCoordinate(int32_t x,int32_t y);



};

class CompositeWidget:public Widget{ 
protected:
	Widget* children[100];
	int children_num;
	Widget* focuessed_child;
public:

	CompositeWidget(Widget* parent,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		uint8_t r,uint8_t g,uint8_t b);

	~CompositeWidget();
	

	virtual void getFocus(Widget* widget);
 	virtual bool addChild(Widget* child);

	virtual void Draw(GraphicsContext* gc);
	virtual void onMouseMove(int32_t old_x,int32_t old_y,int32_t new_x,int32_t new_y);
	virtual void onMouseUp(int32_t x,int32_t y,uint8_t button);
	virtual void onMouseDown(int32_t x,int32_t y,uint8_t button);


	virtual void OnKeyDown(char str);
	virtual void OnKeyUp(char str);
};

#endif