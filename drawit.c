#include <SDL3/SDL.h>
 #include "drawit.h"
 #include<stdio.h>

 bool surface_setup(char **bmp_path,SDL_Surface ** surface){
    SDL_asprintf(bmp_path, "%ssample.bmp", SDL_GetBasePath());  /* allocate a string of the full file path */
     printf("%s",*bmp_path);
     *surface = SDL_LoadBMP(*bmp_path);
     if (!*surface) {
        return false;
         //SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
         //return SDL_APP_FAILURE;
     }
 
     SDL_free(*bmp_path);  /* done with this, the file is loaded. */
     return true;
 
 }

 void process_surface(SDL_Surface * surface){

    int x, y;

         /*   
         
         for (y = 0; y < surface->h; y++) {
             Uint32 *pixels = (Uint32 *) (((Uint8 *) surface->pixels) + (y * surface->pitch));
             for (x = 0; x < surface->w; x++) {
                 Uint8 *p = (Uint8 *) (&pixels[x]);
                 const Uint32 average = (((Uint32) p[1]) + ((Uint32) p[2]) + ((Uint32) p[3])) / 3;
                 if (average == 0) {
                     p[0] = p[3] = 0xFF; p[1] = p[2] = 0;  /* make pure black pixels red. *//*
                 } else {
                     p[1] = p[2] = p[3] = (average > 50) ? 0xFF : 0x00;  /* make everything else either black or white. *//*
                 }
             }
         }
         */
        //Uint32 *pixels = (Uint32 *) (((Uint8 *) surface->pixels));
        for (y = 0; y < surface->h; y++) {
            for (x = 0; x < surface->w; x++) {
                Uint8 *p = (Uint8 *) (&((Uint32 *) (((Uint8 *) surface->pixels) + (y * surface->pitch)))[x]);
        
                // Calcular la intensidad en escala de grises usando el promedio de los canales RGB
                Uint8 grayscale = (Uint8)(((Uint32) p[1]*0.1141f + (Uint32) p[2]*0.587f + (Uint32) p[3]*0.2989f));
                //Uint8 grayscale2 = (Uint8)(((Uint32) p[1] + (Uint32) p[2] + (Uint32) p[3])/3);
                
        
                // Asignar el valor de escala de grises a todos los canales RGB

                p[1] =grayscale; // Azul
                p[2] = grayscale; // Verde
                p[3] = grayscale; // Rojo

            }
        }

 }