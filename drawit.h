#ifndef DRAWIT_GRAPH
#define DRAWIT_GRAPH
#include <SDL3/SDL.h>

bool surface_setup(char **bmp_path,SDL_Surface ** surface);

void process_surface(SDL_Surface * surface);

#endif