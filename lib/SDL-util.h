#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include "SDL.h"


Uint32 get_SDL_pixel(SDL_Surface *surface, int x, int y);
void put_SDL_pixel(SDL_Surface *surface, int x, int y, Uint8 R, Uint8 G, Uint8 B);


#endif // SDL_UTIL_H
