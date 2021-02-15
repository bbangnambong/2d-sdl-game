#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.hpp>

class Entity{
public:
	Entity(Vector2f p_pos, SDL_Texture* p_tex);
	Vector2f& getpos();
	SDL_Texture* gettex();
	SDL_Rect getcurrentFrame();
	void set_x(int x_);
	void set_y(int y_);
	void set_currentFrame(int width, int height);
	void moving(float to_x, float to_y);
	void changetex(SDL_Texture* tex_);
private:
	Vector2f pos;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};