// custom made SDL_Collide.h function by Amir 'Genjix' Taaki released under GNU GPL

//genjix@gmail.com

//available from http://www.angelfire.com/vamp/genjix/sdl_lib/SDL_Collide.h
//referenced and used to create bullet to asteroid collision within my main game loop

#include <SDL.h>
#include <iostream>

//returns maximum or minimum of number
#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)

//returns 1 if offsets are a transparent pixel

//test whether pixels color is equal to the color of transparent pixels for that surface
bool SDL_TransparentPixel(SDL_Surface *collisionsurface, int u, int v)
{
	int bpp = collisionsurface->format->BytesPerPixel;

	//p is the address to the pixel we want to retrieve
	Uint8 *p = (Uint8 *)collisionsurface->pixels + v * collisionsurface->pitch + u * bpp;

	Uint32 pixelcolor;

	switch (bpp)
	{
	case(1) :
		pixelcolor = *p;
		break;

	case(2) :
		pixelcolor = *(Uint16 *)p;
		break;

	case(3) :
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			pixelcolor = p[0] << 16 | p[1] << 8 | p[2];
		else
			pixelcolor = p[0] | p[1] << 8 | p[2] << 16;
		break;

	case(4) :
		pixelcolor = *(Uint32 *)p;
		break;
	}

	//test whether pixels color == color of transparent pixels for that surface
	return SDL_SetColorKey(collisionsurface, SDL_TRUE, pixelcolor);
}

//pixel perfect collision test
int SDL_Collide(SDL_Surface *as, int ax, int ay, SDL_Surface *bs, int bx, int by)
{
	//a - bottom right co-ordinates
	int ax1 = ax + as->w - 1;
	int ay1 = ay + as->h - 1;

	//b bottom right co-ordinates
	int bx1 = bx + bs->w - 1;
	int by1 = by + bs->h - 1;

	//checks if bounding boxes intersect
	if ((bx1 < ax) || (ax1 < bx))
		return 0;
	if ((by1 < ay) || (ay1 < by))
		return 0;

	//loops around every pixel in area of
	//intersection
	//if 2 pixels alpha values on 2 surfaces at the
	//same place != 0 then there is collision
	int inter_x0 = SDL_COLLIDE_MAX(ax, bx);
	int inter_x1 = SDL_COLLIDE_MIN(ax1, bx1);

	int inter_y0 = SDL_COLLIDE_MAX(ay, by);
	int inter_y1 = SDL_COLLIDE_MIN(ay1, by1);

	for (int y = inter_y0; y <= inter_y1; y++)
	{
		for (int x = inter_x0; x <= inter_x1; x++)
		{
			if ((SDL_TransparentPixel(as, x - ax, y - ay))
				&& (SDL_TransparentPixel(bs, x - bx, y - by)))
				return 1;
		}
	}
}

//Bounding box test
int SDL_BoundingCollide(SDL_Surface *sa, int ax, int ay, SDL_Surface *sb, int bx, int by)
{
	if (bx + sb->w < ax)	return 0;	//Checking if their
	if (bx > ax + sa->w)	return 0;	//bounding boxes even touch
	
	if (by + sb->h < ay)	return 0;	
	if (by > ay + sa->h)	return 0;

	return 1;
}

//SDL bounding box collision tests
int SDL_BoundingCollide(SDL_Rect a, SDL_Rect b)
{
	if (b.x + b.w < a.x)	return 0;	//Checking if their
	if (b.x > a.x + a.w)	return 0;	//bounding boxes even touch
	
	if (b.y + b.h < a.y)	return 0;	
	if (b.y > a.y + a.h)	return 0;

	return 1;		
}
