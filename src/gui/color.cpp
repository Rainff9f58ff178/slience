



#include "gui/color.h"



Color::Color(const Color& color){
	this->r=color.r;
	this->g = color.g;
	this->b = color.b;
}

Color::~Color(){

}
Color::Color(){

}
Color::Color(uint8_t r,uint8_t g,uint8_t b){
	this->r=r;
	this->g=g;
	this->b=b;
}