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
#define WEAPON_DELAY 700
#define WEAPON_LAST 80
#define GROUND_LIMIT 500
#define ENEMY_SPEED 3
#define ENEMY_HP 3
#define ENEMY_HIT_DELAY 2000

int main(int argc, char* args[]){
	//Initializing
	if (SDL_Init(SDL_INIT_VIDEO) > 0){
		std::cout << "SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << std::endl;
	}
	if(!(IMG_Init(IMG_INIT_PNG))){
		std::cout << "IMG_INIT has failed. ERROR: " << SDL_GetError() << std::endl;
	}
	RenderWindow window("GAME v1.0", 1280, 720);

	//Loading textures
	SDL_Texture* Backgroundtex = window.loadTexture("res/gfx/s_back.png");
	SDL_Texture* grassTexture = window.loadTexture("res/gfx/s_brick.png");
	SDL_Texture* weapontex[2] = {window.loadTexture("res/gfx/s_weapon_l.png"), window.loadTexture("res/gfx/s_weapon_r.png")};
	SDL_Texture* c_walk_l[7] = {window.loadTexture("res/gfx/c_walk/c_walk_l_1.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_2.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_3.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_4.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_5.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_6.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_7.png")};
	SDL_Texture* c_walk_r[7] = {window.loadTexture("res/gfx/c_walk/c_walk_r_1.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_2.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_3.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_4.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_5.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_6.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_7.png")};
	SDL_Texture* c_jump_r[3] = {window.loadTexture("res/gfx/c_jump/c_jump_r_1.png"), window.loadTexture("res/gfx/c_jump/c_jump_r_2.png"), window.loadTexture("res/gfx/c_jump/c_jump_r_3.png")};
	SDL_Texture* c_jump_l[3] = {window.loadTexture("res/gfx/c_jump/c_jump_l_1.png"), window.loadTexture("res/gfx/c_jump/c_jump_l_2.png"), window.loadTexture("res/gfx/c_jump/c_jump_l_3.png")};
	SDL_Texture* e_walk_l[7] = {window.loadTexture("res/gfx/c_walk/c_walk_l_1.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_2.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_3.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_4.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_5.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_6.png"), window.loadTexture("res/gfx/c_walk/c_walk_l_7.png")};
	SDL_Texture* e_walk_r[7] = {window.loadTexture("res/gfx/c_walk/c_walk_r_1.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_2.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_3.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_4.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_5.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_6.png"), window.loadTexture("res/gfx/c_walk/c_walk_r_7.png")};
	SDL_Texture* e_jump_r[3] = {window.loadTexture("res/gfx/c_jump/c_jump_r_1.png"), window.loadTexture("res/gfx/c_jump/c_jump_r_2.png"), window.loadTexture("res/gfx/c_jump/c_jump_r_3.png")};
	SDL_Texture* e_jump_l[3] = {window.loadTexture("res/gfx/c_jump/c_jump_l_1.png"), window.loadTexture("res/gfx/c_jump/c_jump_l_2.png"), window.loadTexture("res/gfx/c_jump/c_jump_l_3.png")};
	SDL_Texture* enemytex = {window.loadTexture("res/gfx/s_enemy.png")};

	//creating entities
	Entity background = Entity(Vector2f(0, 0), Backgroundtex);
	background.set_currentFrame(SCREEN_WIDTH, SCREEN_HEIGHT);
	std::vector<Entity> grounds = {Entity(Vector2f(700, SCREEN_HEIGHT-300), grassTexture), Entity(Vector2f(100, SCREEN_HEIGHT-128), grassTexture), Entity(Vector2f(300, SCREEN_HEIGHT-200), grassTexture), Entity(Vector2f(500, SCREEN_HEIGHT-250), grassTexture)};
	for(int i = -10; i < SCREEN_WIDTH/32 + 10 ; i++){
		grounds.push_back(Entity(Vector2f(i*32, SCREEN_HEIGHT-64), grassTexture));
	}
	std::vector<Player> players = {Player(Vector2f(SCREEN_WIDTH/2, 0), c_walk_r[0])};
	Player weapon = Player(Vector2f(0,0), weapontex[1]);
	weapon.set_currentFrame(70,40);
	Player enemy1 = Player(Vector2f(0,0), e_walk_r[0]);
	std::vector<Enemy> enemies;


	bool gameRunning = true;
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	float to_x = 0, to_y = 0, e_to_y = 0;
	double e_to_x = 0;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();


	int walknum = 1;
	int e_walknum = 1;
	bool isleft = false;
	bool e_isleft = false;
	int weapon_time = 0;
	int weapon_delay = 0;
	bool isweapon = false;
	int score = 0;
	int spawnTime = 0;
	int jumpTime = 0;
	int enemy1_damaged = 0;
	int enemy1_hit_delay = 0;

	while (gameRunning){
		
		int startTicks = SDL_GetTicks();
		

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= timeStep){
			//Keyboard input
			if (state[SDL_SCANCODE_SPACE]){
				if(players[0].isgrounded()){
					to_y = -players[0].getvy();
					players[0].jump();
				}
			}
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
								if(startTicks - weapon_delay > WEAPON_DELAY){
								isweapon = true;
								weapon_time = startTicks;
								weapon_delay = startTicks;
								}
								break;					
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
		 
		if(enemy1.getpos().x < players[0].getpos().x){
			e_to_x = ENEMY_SPEED;
			e_isleft = false;
			e_walknum++;
			if(e_walknum >= WALKING_MOTION_SPEED*7) e_walknum = WALKING_MOTION_SPEED;
			enemy1.changetex(e_walk_r[e_walknum/WALKING_MOTION_SPEED]);
		}
		else{
			e_to_x = -ENEMY_SPEED;
			e_isleft = true;
			e_walknum++;
			if(e_walknum >= WALKING_MOTION_SPEED*7) e_walknum = WALKING_MOTION_SPEED;
			enemy1.changetex(e_walk_l[e_walknum/WALKING_MOTION_SPEED]);
		}

		//Gravity & enemy1 moving
		if(!players[0].isgrounded()) to_y += GRAVITY ;
		if(!enemy1.isgrounded()) e_to_y += GRAVITY;
		players[0].moving(0, to_y);
		enemy1.moving(e_to_x, e_to_y);

		//Ground collision
		bool jumpu = true;
		bool e_jumpu = true;
		bool e_dojump = false;

		for(unsigned int i = 0; i < grounds.size(); i++){
			//player Ground collision
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

			//enemy1 Ground collision
			int e_collision = enemy1.iscollided(grounds[i], e_to_x, e_to_y);

			if(e_collision == 1){
				enemy1.set_y(grounds[i].getpos().y + grounds[i].getcurrentFrame().h);
				e_to_y = 0;
			}
			if(e_collision == 2){
				enemy1.set_y(grounds[i].getpos().y - enemy1.getcurrentFrame().h);
				enemy1.landed();
				e_to_y = 0;
				e_jumpu = false;
			}
			if(e_collision == 3){
				enemy1.set_x(grounds[i].getpos().x - enemy1.getcurrentFrame().w);
				e_to_x = 0;
				e_dojump = true;
			}
			if(e_collision == 4){
				enemy1.set_x(grounds[i].getpos().x + grounds[i].getcurrentFrame().w);
				e_to_x = 0;
				e_dojump = true;
			}
		}
		if(startTicks - jumpTime> 2500){
			e_to_y = -25;
			jumpTime = startTicks;
		}
		if(e_dojump) e_to_y = -10;
		if(e_jumpu) enemy1.jump();
		if(jumpu) players[0].jump();

		//Tiling
		if((to_x + players[0].getcurrentFrame().w + players[0].getpos().x > SCREEN_WIDTH - GROUND_LIMIT && to_x > 0 )|| (players[0].getpos().x + to_x  < GROUND_LIMIT && to_x < 0)){//Right Limit
			for(unsigned int i = 0; i < grounds.size(); i++){
				grounds[i].moving(-to_x,0);
				Vector2f pos = grounds[i].getpos();
				if(to_x>0 && pos.x < -320){
					grounds.erase(grounds.begin()+i);
					grounds.push_back(Entity(Vector2f(1600, pos.y), grassTexture));
				}
				if(to_x<0 && pos.x > 1600){
					grounds.erase(grounds.begin()+i);
					grounds.push_back(Entity(Vector2f(-320, pos.y), grassTexture));
				}
			}
			for(unsigned int i = 0; i < enemies.size(); i++){
				enemies[i].moving(-to_x,0);
				Vector2f pos = enemies[i].getpos();
				if(to_x>0 && pos.x < -320){
					enemies.erase(enemies.begin()+i);
					enemies.push_back(Enemy(Vector2f(1600, pos.y), enemytex));
				}
				if(to_x<0 && pos.x > 1600){
					enemies.erase(enemies.begin()+i);
					enemies.push_back(Enemy(Vector2f(-320, pos.y), enemytex));
				}
			}
			enemy1.moving(-to_x,0);
			Vector2f pos = enemy1.getpos();
			if(to_x>0 && pos.x < -320){
				enemy1.set_x(pos.x+1600);
			}
			if(to_x<0 && pos.x > 1600){
				enemy1.set_x(pos.x-320);
			}
		}
		else{
			players[0].moving(to_x, 0);
		}


		//Inertia
		to_x *=0.8;

		//Weapon enemy interaction
		if(startTicks - weapon_time > WEAPON_LAST){
			isweapon = false;
		}
		if(isweapon){
			if(isleft){
				weapon.changetex(weapontex[0]);
				weapon.set_x(players[0].getpos().x - weapon.getcurrentFrame().w);
				weapon.set_y(players[0].getpos().y + (players[0].getcurrentFrame().h - weapon.getcurrentFrame().h)/2);
			}
			else{
				weapon.changetex(weapontex[1]);
				weapon.set_x(players[0].getpos().x + players[0].getcurrentFrame().w);
				weapon.set_y(players[0].getpos().y + (players[0].getcurrentFrame().h - weapon.getcurrentFrame().h)/2);
			}
			for(unsigned int i = 0; i < enemies.size(); i++){
				int collision = weapon.iscollided(enemies[i], to_x, to_y);
				if(collision != 0){
					enemies.erase(enemies.begin()+i);
					score++;
					std::cout << "score : " << score << std::endl;
				}
			}
			int collision = weapon.iscollided(enemy1, to_x, to_y);
			if(collision != 0 && startTicks - enemy1_hit_delay > ENEMY_HIT_DELAY){
				enemy1_hit_delay = startTicks;
				if(++enemy1_damaged >= ENEMY_HP){
					enemy1.set_y(2000);
				}
			}
		}

		//Enemies and player collision
		for(unsigned int i = 0; i < enemies.size(); i++){
			int collision = players[0].iscollided(enemies[i], to_x, to_y);
			if(collision != 0) gameRunning = false;
		}

		//Enemies spawn
		if(startTicks - spawnTime> 3000){
			enemies.push_back(Enemy(Vector2f(enemy1.getpos()), enemytex));
			spawnTime = startTicks;
		}

		//Enemies chasing player
		for(unsigned int i = 0; i < enemies.size(); i++){
			float ex=0, ey=0;
			Vector2f p = enemies[i].getdst(players[0].getpos());
			ex = p.x;
			ey = p.y;
			enemies[i].moving(ex, ey);
		}

		
		window.clear();

		//Player jump motions
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

		//Enemy1 jump motions

		if(e_to_y > 0){
			if(!e_isleft) enemy1.changetex(e_jump_r[1]);
			else enemy1.changetex(e_jump_l[1]);
		}
		if(e_to_y < 0){
			if(!e_isleft) enemy1.changetex(e_jump_r[2]);
			else enemy1.changetex(e_jump_l[2]);
		}
		if(abs(e_to_x) < 2 && to_y == 0){
			if(e_isleft) enemy1.changetex(e_walk_l[0]);
			else enemy1.changetex(e_walk_r[0]);
		}

		//Rendering
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
		window.render(enemy1);
		if(isweapon){
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