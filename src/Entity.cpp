#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.hpp>

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex) : pos(p_pos), tex(p_tex) {
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 64;
	currentFrame.h = 64;
}

SDL_Texture* Entity::gettex(){
	return tex;
}

SDL_Rect Entity::getcurrentFrame(){
	return currentFrame;
}

Vector2f& Entity::getpos(){
	return pos;
}

void Entity::set_x(int x_){
	pos.x = x_;
}

void Entity::set_y(int y_){
	pos.y = y_;
}

void Entity::set_currentFrame(int width, int height){
	currentFrame.w = width;
	currentFrame.h = height;
}

void Entity::moving(float to_x, float to_y){
	pos.x += to_x;
	pos.y += to_y;
}

void Entity::changetex(SDL_Texture* tex_){
	tex = tex_;
}