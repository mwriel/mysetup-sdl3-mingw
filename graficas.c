/* read-pixels.c ... */

/*
 * This example creates an SDL window and renderer, and draws a
 * rotating texture to it, reads back the rendered pixels, converts them to
 * black and white, and then draws the converted image to a corner of the
 * screen.
 *
 * This isn't necessarily an efficient thing to do--in real life one might
 * want to do this sort of thing with a render target--but it's just a visual
 * example of how to use SDL_RenderReadPixels().
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

 #define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
 #include <SDL3/SDL.h>
 #include <SDL3/SDL_main.h>
 #include<stdio.h>
 //#include"drawit.h"
 
 #include"graficas.h"

 
 
 
 
 /* We will use this renderer to draw into this window every frame. */
 static SDL_Window *window = NULL;
 static SDL_Renderer *renderer = NULL;
 static SDL_Texture *texture = NULL;
 static int texture_width = 0;
 static int texture_height = 0;
 static SDL_Texture *converted_texture = NULL;
 static int converted_texture_width = 0;
 static int converted_texture_height = 0;
 
 #define WINDOW_WIDTH 640*2
 #define WINDOW_HEIGHT 480
 
 /* This function runs once at startup. */
 SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
 {
     SDL_Surface *surface = NULL;
     char *bmp_path = NULL;
 
     SDL_SetAppMetadata("Example Renderer Read Pixels", "1.0", "com.example.renderer-read-pixels");
 
     if (!SDL_Init(SDL_INIT_VIDEO)) {
         SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }
 
     if (!SDL_CreateWindowAndRenderer("examples/renderer/read-pixels", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
         SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }
 
     /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
        engines refer to these as "sprites." We'll do a static texture (upload once, draw many
        times) with data from a bitmap file. */

        if(!surface_setup(&bmp_path,&surface)){
            SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
         return SDL_APP_FAILURE;
        }
 
     /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
        Load a .bmp into a surface, move it to a texture from there. */
     
     texture_width = surface->w;
     texture_height = surface->h;
     //process_surface(surface);
     
 
     texture = SDL_CreateTextureFromSurface(renderer, surface);
     if (!texture) {
         SDL_Log("Couldn't create static texture: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }
 
     SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs when a new event (mouse input, keypresses, etc) occurs. */
 SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
 {
     if (event->type == SDL_EVENT_QUIT) {
         return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
     }
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once per frame, and is the heart of the program. */
 SDL_AppResult SDL_AppIterate(void *appstate)
 {
     const Uint64 now = SDL_GetTicks();
     SDL_Surface *surface;
     SDL_FPoint center;
     SDL_FRect dst_rect;
 
     /* we'll have a texture rotate around over 2 seconds (2000 milliseconds). 360 degrees in a circle! */
     const float rotation = (((float) ((int) (now % 2000))) / 2000.0f) * 360.0f;
 
     /* as you can see from this, rendering draws over whatever was drawn before it. */
     SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
     SDL_RenderClear(renderer);  /* start with a blank canvas. */
 
     /* Center this one, and draw it with some rotation so it spins! */
     dst_rect.x = ((float) ((WINDOW_WIDTH/2.0f) - texture_width)) / 2.0f;
     dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
     dst_rect.w = (float) texture_width;
     dst_rect.h = (float) texture_height;
     /* rotate it around the center of the texture; you can rotate it from a different point, too! */
     center.x = texture_width / 2.0f;
     center.y = texture_height / 2.0f;
     //SDL_RenderTextureRotated(renderer, texture, NULL, &dst_rect, rotation, &center, SDL_FLIP_NONE);
     SDL_RenderTexture(renderer, texture, NULL, &dst_rect);
 
     /* this next whole thing is _super_ expensive. Seriously, don't do this in real life. */
 
     /* Download the pixels of what has just been rendered. This has to wait for the GPU to finish rendering it and everything before it,
        and then make an expensive copy from the GPU to system RAM! */
     surface = SDL_RenderReadPixels(renderer, NULL);
 
     /* This is also expensive, but easier: convert the pixels to a format we want. */
     if (surface && (surface->format != SDL_PIXELFORMAT_RGBA8888) && (surface->format != SDL_PIXELFORMAT_BGRA8888)) {
         SDL_Surface *converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
         SDL_DestroySurface(surface);
         surface = converted;
     }
 
     if (surface) {
         /* Rebuild converted_texture if the dimensions have changed (window resized, etc). */
         if ((surface->w != converted_texture_width) || (surface->h != converted_texture_height)) {
             SDL_DestroyTexture(converted_texture);
             converted_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);
             if (!converted_texture) {
                 SDL_Log("Couldn't (re)create conversion texture: %s", SDL_GetError());
                 return SDL_APP_FAILURE;
             }
             converted_texture_width = surface->w;
             converted_texture_height = surface->h;
         }
 
         /* Turn each pixel into either black or white. This is a lousy technique but it works here.
            In real life, something like Floyd-Steinberg dithering might work
            better: https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering*/

            process_surface(surface);


        /*
        la distancia existente entre dos pixeles con aplicaciones entre similitud y otros casos
        existen varios tipos de medidas para determinaar
        sqrt((x-x)^2+(y-y)^2)
        */

         
 
         /* upload the processed pixels back into a texture. */
         SDL_UpdateTexture(converted_texture, NULL, surface->pixels, surface->pitch);
         SDL_DestroySurface(surface);
 
         /* draw the texture to the top-left of the screen. */
         //dst_rect.x = dst_rect.y = 0.0f;
         dst_rect.x = dst_rect.x+texture_width;
         dst_rect.y=0;
         
         dst_rect.w = ((float) (WINDOW_WIDTH));
         dst_rect.h = ((float) WINDOW_HEIGHT);
         SDL_RenderTexture(renderer, converted_texture, NULL, &dst_rect);
     }
 
     SDL_RenderPresent(renderer);  /* put it all on the screen! */
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once at shutdown. */
 void SDL_AppQuit(void *appstate, SDL_AppResult result)
 {
     SDL_DestroyTexture(converted_texture);
     SDL_DestroyTexture(texture);
     /* SDL will clean up the window/renderer for us. */
 }
