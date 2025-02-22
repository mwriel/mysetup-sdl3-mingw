#include <SDL3/SDL.h>
#include <stdio.h>
#include "graficas.h"

bool surface_setup(char **bmp_path, SDL_Surface **surface)
{
    SDL_asprintf(bmp_path, "%ssample.bmp", SDL_GetBasePath()); /* asignar la string full file path */
    printf("%s", *bmp_path);
    *surface = SDL_LoadBMP(*bmp_path);
    if (!*surface)
    {
        return false;
        // revise este bolleno por ejemplo asi
        // SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        // return SDL_APP_FAILURE;
    }

    SDL_free(*bmp_path); /* done with this, the file is loaded. */
    return true;
}

void process_surface(SDL_Surface *surface)
{

    int x, y;

    // Uint32 *pixels = (Uint32 *) (((Uint8 *) surface->pixels));
    for (y = 0; y < surface->h; y++)
    {
        for (x = 0; x < surface->w; x++)
        {
            Uint8 *p = (Uint8 *)(&((Uint32 *)(((Uint8 *)surface->pixels) + (y * surface->pitch)))[x]);

            // Calcular la intensidad en escala de grises usando el promedio los canales RGB para binarizar
            Uint8 grayscale = (Uint8)(((Uint32)p[1]  + (Uint32)p[2] + (Uint32)p[3])/3);
            // Uint8 grayscale2 = (Uint8)(((Uint32) p[1] + (Uint32) p[2] + (Uint32) p[3])/3);

            // Asignar el valor de escala de grises a todos los canales RGB

            p[1] = grayscale; // Azul
            p[2] = grayscale; // Verde
            p[3] = grayscale; // Rojo
        }
    }
}