#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

class Player : public Entity{
public:
	Player(Vector2f p_pos, SDL_Texture* p_tex);
	void jump();
	float getvx();
	float getvy();
	bool isgrounded();
	void landed();
private:
	float vel_x;
	float vel_y;
	bool grounded;
};