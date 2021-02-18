#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "Enemy.hpp"
#include "Entity.hpp"

Enemy::Enemy(Vector2f p_pos, SDL_Texture* p_tex) : Entity(p_pos, p_tex){
	vel= 6;
	set_currentFrame(30,30);
}

Vector2f Enemy::getdst(Vector2f a_pos){
	Vector2f temp;
	float par = vel / sqrt(pow(abs(a_pos.x - getpos().x),2) + pow(abs(a_pos.y - getpos().y),2));
	temp.x = (a_pos.x - getpos().x) * par;
	temp.y = (a_pos.y - getpos().y) * par;

	return temp;
}

