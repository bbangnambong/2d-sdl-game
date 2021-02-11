#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Player.hpp"
#include "Entity.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Player::Player(Vector2f p_pos, SDL_Texture* p_tex) : Entity(p_pos, p_tex) {
	grounded = false;
	vel_x = 5;
	vel_y = 20;
	set_currentFrame(50, 100);
	set_x(SCREEN_WIDTH/2 - getcurrentFrame().w/2);
	set_y(0);
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
