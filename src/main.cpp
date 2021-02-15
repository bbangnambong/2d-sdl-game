#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "math.hpp"
#include "utils.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.6
#define WALKING_MOTION_SPEED 10
#define JUMP_MOTION_SPEED 10


int main(int argc, char* args[]){
	if (SDL_Init(SDL_INIT_VIDEO) > 0){
		std::cout << "SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << std::endl;
	}
	if(!(IMG_Init(IMG_INIT_PNG))){
		std::cout << "IMG_INIT has failed. ERROR: " << SDL_GetError() << std::endl;
	}

	RenderWindow window("GAME v1.0", 1280, 720);

	SDL_Texture* Backgroundtex = window.loadTexture("res/gfx/s_back.png");
	SDL_Texture* grassTexture = window.loadTexture("res/gfx/s_brick.png");
	SDL_Texture* weapontex = window.loadTexture("res/gfx/s_weapon_L.png");
	SDL_Texture* c_walk_l[7] = {window.loadTexture("res/gfx/c_walk/c_walk_l_1.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_2.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_3.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_4.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_5.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_6.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_7.png")};
	SDL_Texture* c_walk_r[7] = {window.loadTexture("res/gfx/c_walk/c_walk_r_1.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_2.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_3.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_4.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_5.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_6.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_7.png")};
	SDL_Texture* c_jump_r[3] = {window.loadTexture("res/gfx/c_jump/c_jump_r_1.png"), window.loadTexture("res/gfx/c_jump/c_jump_r_2.png"), window.loadTexture("res/gfx/c_jump/c_jump_r_3.png")};
	SDL_Texture* c_jump_l[3] = {window.loadTexture("res/gfx/c_jump/c_jump_l_1.png"), window.loadTexture("res/gfx/c_jump/c_jump_l_2.png"), window.loadTexture("res/gfx/c_jump/c_jump_l_3.png")};
	SDL_Texture* enemytex = {window.loadTexture("res/gfx/s_enemy.png")};


	Entity background = Entity(Vector2f(0, 0), Backgroundtex);
	background.set_currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);

	



	std::vector<Entity> grounds = {Entity(Vector2f(700, SCREEN_HEIGHT-300), grassTexture), Entity(Vector2f(100, SCREEN_HEIGHT-128), grassTexture), Entity(Vector2f(300, SCREEN_HEIGHT-200), grassTexture), Entity(Vector2f(500, SCREEN_HEIGHT-250), grassTexture)};;
	for(int i = 0; i < SCREEN_WIDTH/64 ; i++){
		grounds.push_back(Entity(Vector2f(i*64, SCREEN_HEIGHT-64), grassTexture));
	}
	std::vector<Player> players = {Player(Vector2f(0, 0), c_walk_r[0])};
	Entity weapon = Entity(Vector2f(0,0), weapontex);
	std::vector<Enemy> enemies;



	bool gameRunning = true;
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	float to_x = 0, to_y = 0;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();


	int walknum = 1;
	bool isleft = false;


	while (gameRunning){
		
		bool isweapon = false;
		int startTicks = SDL_GetTicks();
		std::cout << startTicks << std::endl;

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= timeStep){

			if (state[SDL_SCANCODE_SPACE]){
				if(players[0].isgrounded()){
					to_y = -players[0].getvy();
					players[0].jump();
				}
			}
			/*if (state[SDL_SCANCODE_KP_A]){
				isweapon = true;
			}*/
			if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]){
				walknum++;
				if(walknum >= WALKING_MOTION_SPEED*7) walknum = WALKING_MOTION_SPEED;
				players[0].changetex(c_walk_r[walknum/WALKING_MOTION_SPEED]);
				to_x += players[0].getvx();
				isleft = false;
			}
			if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]){
				walknum++;
				if(walknum >= WALKING_MOTION_SPEED*7) walknum = WALKING_MOTION_SPEED;
				players[0].changetex(c_walk_l[walknum/WALKING_MOTION_SPEED]);
				to_x -= players[0].getvx();
				isleft = true;
			}

			while (SDL_PollEvent(&event)){
				switch (event.type){
					case SDL_QUIT:
						gameRunning = false;
						break;
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym ){
							case SDLK_a:
								isweapon = true;							
						}
						break;
					default:
						break;

				}
			}
			//limiting character velocity
			if(to_x > players[0].getvx()) to_x = players[0].getvx();
			if(to_x < -players[0].getvx()) to_x = -players[0].getvx();



			accumulator -= timeStep;
		}
		float alpha = accumulator / timeStep;
		
		
		if(!players[0].isgrounded()) to_y += GRAVITY;

		players[0].moving(0, to_y);
		//collision
		bool jumpu = true;
		for(unsigned int i = 0; i < grounds.size(); i++){
			int collision = players[0].iscollided(grounds[i], to_x, to_y);
			
			if(collision == 1){
				players[0].set_y(grounds[i].getpos().y + grounds[i].getcurrentFrame().h);
				to_y = 0;
			}
			if(collision == 2){
				players[0].set_y(grounds[i].getpos().y - players[0].getcurrentFrame().h);
				players[0].landed();
				to_y = 0;
				jumpu = false;
			}
			if(collision == 3){
				players[0].set_x(grounds[i].getpos().x - players[0].getcurrentFrame().w);
				to_x = 0;
			}
			if(collision == 4){
				players[0].set_x(grounds[i].getpos().x + grounds[i].getcurrentFrame().w);
				to_x = 0;
			}
		}
		if(jumpu){
			players[0].jump();
		}
		players[0].moving(to_x, 0);
		
		to_x *=0.9;

	for(unsigned int i = 0; i < enemies.size(); i++){
		int collision = players[0].iscollided(enemies[i], to_x, to_y);
		if(collision != 0) gameRunning = false;
	}



		if(startTicks/3000 - enemies.size() > 0){
			enemies.push_back(Enemy(Vector2f(rand()%1280,0), enemytex));
		}
		for(unsigned int i = 0; i < enemies.size(); i++){
			float ex=0, ey=0;
			Vector2f p = enemies[i].getdst(players[0].getpos());
			ex = p.x;
			ey = p.y;
			enemies[i].moving(ex, ey);
		}
		

		window.clear();
		if(to_y > 0){
			if(!isleft) players[0].changetex(c_jump_r[1]);
			else players[0].changetex(c_jump_l[1]);
		}
		if(to_y < 0){
			if(!isleft) players[0].changetex(c_jump_r[2]);
			else players[0].changetex(c_jump_l[2]);
		}
		if(abs(to_x) < 2 && to_y == 0){
			if(isleft) players[0].changetex(c_walk_l[0]);
			else players[0].changetex(c_walk_r[0]);
		}

		window.render(background);
		for(unsigned int i = 0; i < grounds.size(); i++){
			window.render(grounds[i]);
		}
		for(unsigned int i = 0; i < players.size(); i++){
			window.render(players[i]);
		}
		for(unsigned int i = 0; i < enemies.size(); i++){
			window.render(enemies[i]);
		}
		if(isweapon){
			weapon.set_x(players[0].getpos().x - weapon.getcurrentFrame().w);
			weapon.set_y(players[0].getpos().y);
			window.render(weapon);
		}

		window.display();

		int frameTicks = SDL_GetTicks() - startTicks;

		if (frameTicks < 1000 / window.getRefreshRate()){
			SDL_Delay(1000/ window.getRefreshRate() - frameTicks);
		}
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}