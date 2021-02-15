#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "RenderWindow.hpp"
#include "Entity.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) :window(NULL), renderer(NULL) {
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL){
		std::cout << "Window failed to intit. Error: " << SDL_GetError() << std::endl;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath){
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL){
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
	}
	return texture;
}

int RenderWindow::getRefreshRate(){
	int displayIndex = SDL_GetWindowDisplayIndex(window);
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(displayIndex, 0, &mode);
	return mode.refresh_rate;
}

void RenderWindow::cleanUp(){
	SDL_DestroyWindow(window);
}

void RenderWindow::clear(){
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity){
	SDL_Rect src;
	src.x = p_entity.getcurrentFrame().x;
	src.y = p_entity.getcurrentFrame().y;
	src.w = p_entity.getcurrentFrame().w;
	src.h = p_entity.getcurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getpos().x * 1;
	dst.y = p_entity.getpos().y * 1;
	dst.w = p_entity.getcurrentFrame().w * 1;
	dst.h = p_entity.getcurrentFrame().h * 1;

	SDL_RenderCopy(renderer, p_entity.gettex(), &src, &dst);
}

void RenderWindow::display(){
	SDL_RenderPresent(renderer);
}
