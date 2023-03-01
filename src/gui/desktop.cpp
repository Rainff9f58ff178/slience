
#include "gui/desktop.h"



Desktop::Desktop(uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b):CompositeWidget(0,0,0,w,h,r,g,b),MouseEventHandler(){
	mouse_x=w/2;
	mouse_y=h/2;
}
Desktop::~Desktop(){

}


//CompositeWidget wont draw the mouse,wo we nedd add the function
void Desktop::Draw(GraphicsContext* gc){
	CompositeWidget::Draw(gc);
	for(int i = 0; i < 4; i++)
    {
        gc -> PutPixel(mouse_x-i, mouse_y, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(mouse_x+i, mouse_y, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(mouse_x, mouse_y-i, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(mouse_x, mouse_y+i, 0xFF, 0xFF, 0xFF);
    }
}



void Desktop::OnMouseMove(int x,int y){
	x /=4;
	y /=4;
	int32_t new_mouse_x=mouse_x+x;
	if(new_mouse_x < 0) new_mouse_x=0;
	if(new_mouse_x >= w) new_mouse_x=w-1;


	int32_t new_mouse_y=mouse_y+y;
	if(new_mouse_y < 0) new_mouse_y=0;
	if(new_mouse_y >= h) new_mouse_y=h-1;

	CompositeWidget::onMouseMove(mouse_x,mouse_y,new_mouse_x,new_mouse_y);
    
	mouse_x=new_mouse_x;
	mouse_y=new_mouse_y;

}
void Desktop::OnMouseDown(uint8_t button){
	CompositeWidget::onMouseDown(mouse_x,mouse_y,button);
}
void Desktop::OnMouseUp(uint8_t button){
	CompositeWidget::onMouseUp(mouse_x,mouse_y,button);
}
