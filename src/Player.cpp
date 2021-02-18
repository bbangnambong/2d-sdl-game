#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Player.hpp"
#include "Entity.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Player::Player(Vector2f p_pos, SDL_Texture* p_tex) : Entity(p_pos, p_tex) {
	grounded = false;
	vel_x = 7;
	vel_y = 16;
	set_currentFrame(50, 50);
}

void Player::jump(){
	grounded = false;
}

float Player::getvx(){
	return vel_x;
}

float Player::getvy(){
	return vel_y;
}

bool Player::isgrounded(){
	return grounded;
}

void Player::landed(){
	grounded = true;
}
int Player::iscollided(Entity a, float to_x, float to_y){
	float man_left, man_top, man_bot, man_right;
	float ob_left, ob_top, ob_bot, ob_right;
	man_left = getpos().x ;
	man_right = man_left + getcurrentFrame().w;
	man_top = getpos().y ;
	man_bot = man_top + getcurrentFrame().h;
	ob_left = a.getpos().x;
	ob_right = ob_left + a.getcurrentFrame().w;
	ob_top = a.getpos().y;
	ob_bot = ob_top + a.getcurrentFrame().h;

	
	if((man_top < ob_bot && man_bot > ob_bot && man_top > ob_bot-10) && (man_right > ob_left && man_left < ob_right) && to_y<0) return 1;
	if((man_top < ob_top && man_bot > ob_top && man_bot < ob_top+20) && (man_right > ob_left && man_left < ob_right) && to_y>0) return 2;
	if((man_top < ob_bot && man_bot > ob_top) && (man_right > ob_left && man_right < ob_right))	return 3;//hitting left
	if((man_top < ob_bot && man_bot > ob_top) && (man_left > ob_left && man_left < ob_right)) return 4;//hitting right
	if((man_top < ob_bot && man_bot > ob_top) && (man_right > ob_left && man_left < ob_right)) return 5;// for weapon
	
	return 0;
}