#include <SDL.h>
#include "bullet.h"

//This is a header file i created using tutorials found from
//the website: http://lazyfoo.net/SDL_tutorials/
//i adapted it to fit my own needs, but the principle of its
//functions were created by the author of the website.


//These booleans are used to determine if the ship has entered the vicinity of an asteroid

//they work by looking at points on the ship and asteroid, and creating a shape between these points
//if the shape around the ship intersects the shape around an asteroid, the booleans returns 1

//then in the main game loop, i can referance these boolean functions, inputting the SDL_Rect's of 
//the ship and the asteroid and creating an output depending on whether the boolean has returned 1 or not.
bool shipcollision(SDL_Rect* shipRect, SDL_Rect* rockRect)
{
	if (shipRect->y >= rockRect->y + (rockRect->h - 20))
		return 0;
	if (shipRect->x >= rockRect->x + (rockRect->w - 20))
		return 0;
	if (shipRect->y + shipRect->h <= (rockRect->y + 20))
		return 0;
	if (shipRect->x + shipRect->w <= (rockRect->x + 20))
		return 0;
	return 1;
}
bool shipcollision2(SDL_Rect* shipRect, SDL_Rect* rockRect2)
{
	if (shipRect->y >= rockRect2->y + (rockRect2->h - 20))
		return 0;
	if (shipRect->x >= rockRect2->x + (rockRect2->w - 20))
		return 0;
	if (shipRect->y + shipRect->h <= (rockRect2->y + 20))
		return 0;
	if (shipRect->x + shipRect->w <= (rockRect2->x + 20))
		return 0;
	return 1;
}
bool shipcollision3(SDL_Rect* shipRect, SDL_Rect* rockRect3)
{
	if (shipRect->y >= rockRect3->y + (rockRect3->h - 20))
		return 0;
	if (shipRect->x >= rockRect3->x + (rockRect3->w - 20))
		return 0;
	if (shipRect->y + shipRect->h <= (rockRect3->y + 20))
		return 0;
	if (shipRect->x + shipRect->w <= (rockRect3->x + 20))
		return 0;
	return 1;
}
bool shipcollision4(SDL_Rect* shipRect, SDL_Rect* rockRect4)
{
	if (shipRect->y >= rockRect4->y + (rockRect4->h - 20))
		return 0;
	if (shipRect->x >= rockRect4->x + (rockRect4->w - 20))
		return 0;
	if (shipRect->y + shipRect->h <= (rockRect4->y + 20))
		return 0;
	if (shipRect->x + shipRect->w <= (rockRect4->x + 20))
		return 0;
	return 1;
}
bool shipcollision5(SDL_Rect* shipRect, SDL_Rect* rockRect5)
{
	if (shipRect->y >= rockRect5->y + (rockRect5->h - 20))
		return 0;
	if (shipRect->x >= rockRect5->x + (rockRect5->w - 20))
		return 0;
	if (shipRect->y + shipRect->h <= (rockRect5->y + 20))
		return 0;
	if (shipRect->x + shipRect->w <= (rockRect5->x + 20))
		return 0;
	return 1;
}
bool shipcollision6(SDL_Rect* shipRect, SDL_Rect* rockRect6)
{
	if (shipRect->y >= rockRect6->y + (rockRect6->h - 20))
		return 0;
	if (shipRect->x >= rockRect6->x + (rockRect6->w - 20))
		return 0;
	if (shipRect->y + shipRect->h <= (rockRect6->y + 20))
		return 0;
	if (shipRect->x + shipRect->w <= (rockRect6->x + 20))
		return 0;
	return 1;
}


