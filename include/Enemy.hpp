#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.hpp>

#include "Entity.hpp"

class Enemy : public Entity{
public:
	Enemy(Vector2f p_pos, SDL_Texture* p_tex);
	Vector2f getdst(Vector2f a_pos);
private:
	float vel;
};