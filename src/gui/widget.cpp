#include "gui/widget.h"




Widget::Widget(Widget* parent,
	int32_t x,
	int32_t y,
	int32_t w,
	int32_t h,
	uint8_t r,uint8_t g,uint8_t b){
	this->parent=parent;
	this->x=x;
	this->y=y;
	this->w=w;
	this->h=h;
	this->r=r;
	this->g=g;
	this->b=b;
	this->focuessable=true;
}

Widget::~Widget(){

}
void Widget::getFocus(Widget* widget){
	if(parent!=0)
		parent->getFocus(widget);
}
void Widget::modelToScreen(int32_t& x,int32_t& y){
	if(parent !=0)
		parent->modelToScreen(x,y);
	x+=this->x;
	y+=this->y;
}


void Widget::Draw(GraphicsContext* gc){
	int X=0;
	int Y=0;
	modelToScreen(X,Y);
	gc->FillRectangle(X,Y,w,h,r,g,b);
}
void Widget::onMouseMove(int32_t old_x,int32_t old_y,int32_t new_x,int32_t new_y){

}
void Widget::onMouseUp(int32_t x,int32_t y,uint8_t button){

}
void Widget::onMouseDown(int32_t x,int32_t y,uint8_t button){
	if(this->focuessable)
		getFocus(this);
}

void Widget::OnKeyDown(char str){

}
void Widget::OnKeyUp(char str){

}


bool Widget::containCoordinate(int32_t x,int32_t y){
	return this->x<=x && x<this->x+this->w 
        && this->y<=y && y<this->y+this->h;
}



//class CompositeWidget
CompositeWidget::CompositeWidget(Widget* parent,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		uint8_t r, uint8_t g, uint8_t b):Widget(parent,x,y,w,h,r,g,b){

	this->focuessed_child = 0;
	this->children_num=0;
}

CompositeWidget::~CompositeWidget(){

}

void CompositeWidget::getFocus(Widget* widget){
	this->focuessed_child=widget;
	if(parent!=0)
		parent->getFocus(this);
}


void CompositeWidget::Draw(GraphicsContext* gc){
	Widget::Draw(gc);
	for(int i=children_num-1;i>=0;--i){
		this->children[i]->Draw(gc);
	}
}
void printf(char* str);
void CompositeWidget::onMouseMove(int32_t old_x,int32_t old_y,int32_t new_x,int32_t new_y){
	int first_child=-1;
	for(int i=0;i<children_num;++i){
		if(children[i]->containCoordinate(old_x -this->x,old_y-this->y)){
			this->children[i]->onMouseMove(old_x -this->x,old_y-this->y,new_x-this->x,new_y-this->y);
			first_child=i;
			break;
		}
	}
	for(int i=0;i<children_num;++i){
		if(children[i]->containCoordinate(new_x-this->x,new_y-this->y)){
			if(first_child!=i){
				this->children[i]->onMouseMove(old_x -this->x,old_y-this->y,new_x-this->x,new_y-this->y);
			}
			break;
		}
	}

}
void CompositeWidget::onMouseUp(int32_t x,int32_t y,uint8_t button){
	for(int i=0;i<children_num;++i){
		if(children[i]->containCoordinate(x -this->x,y-this->y)){                                                                              
			this->children[i]->onMouseUp(x -this->x,y-this->y,button);
			break;
		}
	}
}
void CompositeWidget::onMouseDown(int32_t x,int32_t y,uint8_t button){
	for(int i=0;i<children_num;++i){
		if(children[i]->containCoordinate(x -this->x,y-this->y)){
			this->children[i]->onMouseDown(x -this->x,y-this->y,button);
			break;
		}
	}
}

void CompositeWidget::OnKeyDown(char str){
	if(this->focuessed_child!=0)
		this->focuessed_child->OnKeyDown(str);   
}
void CompositeWidget::OnKeyUp(char str){
	if(this->focuessed_child!=0)
		this->focuessed_child->OnKeyUp(str);
}
bool CompositeWidget::addChild(Widget* child){
	if(children_num>=100)
		return false;
	children[children_num++]=child;
	return true;
}
