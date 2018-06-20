#include <iostream>
#include "bullet.h"

//Bullet functions
Bullet::Bullet()
{
	bullettexture = NULL; 
}
Bullet::~Bullet()
{
	if (bullettexture)
	{
		SDL_DestroyTexture(bullettexture);
	}
}

//loadBullet function
bool Bullet::loadBullet(std::string bullet, SDL_Renderer *renderer)	//this sets what happens when the loadBullet function is called
{
	SDL_Surface *bullettemp = SDL_LoadBMP(bullet.c_str());				//the bullet surface is applied

	bullettexture = SDL_CreateTextureFromSurface(renderer, bullettemp); //and then converted to a texture using the SDL function

	return true;
}

//drawBullet function
void Bullet::drawBullet(int posX, int posY, SDL_Renderer *renderer, double angle, SDL_Point *centre)	//this sets what happens when drawBullet is called
{
	SDL_Rect bulletRect;		//uses an SDL_Rect function

	bulletRect.x = posX;	//and sets it axis position
	bulletRect.y = posY;	//both x and y
	bulletRect.w = 3;		//and the size
	bulletRect.h = 3;		//of the bullet

	SDL_RenderCopyEx(renderer, bullettexture, NULL, &bulletRect, angle, centre, SDL_FLIP_NONE); //this function then applies the bullet texture onto the bullet rectangle just created
}

//create bullet function
Bullet *createBullet(float x, float y, float angle, SDL_Renderer *renderer)	//sets what happens when createBullet is called
{
	Bullet *bullet = new Bullet();					//makes a new bullet using the Bullet class
	bullet->xPos = x;								//sets the bullets x
	bullet->yPos = y;								//and y position
	bullet->angle = angle;							//determines its angle
	bullet->killTime = 0;							//tells it to use the killTime
	bullet->loadBullet("bullet.bmp", renderer);		//tells it the name of the bitmap image to get the texture from
	return bullet;
}


