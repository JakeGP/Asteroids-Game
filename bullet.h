#ifndef __BULLET_H__
#define __BULLET_H__

#include <string>
#include <SDL.h>

//A bullet class created to make an array of bullets
class Bullet
{
public:

	Bullet();
	~Bullet();

	float xPos, yPos, angle, renderer;	//this floats the dimensions and specifics of the bullet
	float killTime;						//floatrs a killTime, used to destroy the bullet after a certain amount of Milliseconds

	bool loadBullet(std::string bullet, SDL_Renderer *renderer);	//load bullet function

	void drawBullet(int posX, int posY, SDL_Renderer *renderer, double angle = 0.0, SDL_Point* centre = NULL);	//function that draws the bullet using SDL_Renderer

protected:

	SDL_Texture *bullettexture;	//the texture of the bullet
};

const int NUM_Bullets = 32;				//constant integer to set the maximum number of bullets in the array
extern Bullet *bullets[NUM_Bullets];

Bullet *createBullet(float x, float y, float angle, SDL_Renderer *renderer);

#endif