


#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <stdio.h>
#include <SDL3/SDL_main.h>
#include "practica.h"

static winnode *win1 = NULL;
static winnode *win2 = NULL;
static winnode *win3 = NULL;

static SDL_Surface *imageSurface = NULL; // Superficie para manipulación
static SDL_Texture *imageTexture = NULL; // Textura para renderizado

static SDL_Camera *camera = NULL;
static SDL_Texture *camtexture = NULL;


static winlist *list;

void win1it(winnode *win3n){

}
void win2it(winnode *win3n){

}


void win3it(winnode *win3n){

    Uint64 timestampNS = 0;
    SDL_Surface *frame = SDL_AcquireCameraFrame(camera, &timestampNS);
    winnode * win3o=(win3n );

    if (frame != NULL) {
        /* Some platforms (like Emscripten) don't know _what_ the camera offers
           until the user gives permission, so we build the texture and resize
           the window when we get a first frame from the camera. */
        if (!camtexture) {
            SDL_SetWindowSize(win3o->window, frame->w, frame->h);  /* Resize the window to match */
            camtexture = SDL_CreateTexture(win3o->renderer, frame->format, SDL_TEXTUREACCESS_STREAMING, frame->w, frame->h);
        }

        if (camtexture) {
            SDL_UpdateTexture(camtexture, NULL, frame->pixels, frame->pitch);
        }

        SDL_ReleaseCameraFrame(camera, frame);
    }
    
    
    SDL_SetRenderDrawColor(win3o->renderer, 0x99, 0x99, 0x99, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(win3o->renderer);
    if (camtexture) {  /* draw the latest camera frame, if available. */
        SDL_RenderTexture(win3o->renderer, camtexture, NULL, NULL);
    }
    SDL_RenderPresent(win3o->renderer);

}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CAMERA))
    {
        SDL_Log("no se pudo inicializar video: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_CameraID *devices = NULL;
    int devcount = 0;

    devices = SDL_GetCameras(&devcount);
    if (devices == NULL) {
        SDL_Log("Couldn't enumerate camera devices: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    } else if (devcount == 0) {
        SDL_Log("Couldn't find any camera devices! Please connect a camera and try again.");
        return SDL_APP_FAILURE;
    }

    camera = SDL_OpenCamera(devices[0], NULL);  // just take the first thing we see in any format it wants.
    SDL_free(devices);
    if (camera == NULL) {
        SDL_Log("Couldn't open camera: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    initWinlist(&list);

    char *bmpfile = NULL;

    SDL_asprintf(&bmpfile, "%ssample.bmp", SDL_GetBasePath());

    

    imageSurface = SDL_LoadBMP("sample.bmp");
    if (!imageSurface)
    {
        SDL_Log("Error al cargar la imagen: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (imageSurface && (imageSurface->format != SDL_PIXELFORMAT_RGBA8888) && (imageSurface->format != SDL_PIXELFORMAT_BGRA8888))
    {
        SDL_Surface *converted = SDL_ConvertSurface(imageSurface, SDL_PIXELFORMAT_RGBA8888);
        SDL_DestroySurface(imageSurface);
        imageSurface = converted;
    }

    Uint32 *pixels = (Uint32 *)(((Uint8 *)imageSurface->pixels));
    int pixelCount = imageSurface->w * imageSurface->h;
    SDL_Surface * copySurface=SDL_DuplicateSurface(imageSurface);

    for (int i = 0; i < pixelCount; i++)
    {

        Uint8 *p = (Uint8 *)(&pixels[i]);
        // Calcular la intensidad en escala de grises usando el porcentaje visible de cada uno de los canales RGB
        Uint8 grayscale = (Uint8)(((Uint32)p[1] * 0.1141f + (Uint32)p[2] * 0.587f + (Uint32)p[3] * 0.2989f));
        p[1] = grayscale; // Azul
        p[2] = grayscale; // Verde
        p[3] = grayscale; // Rojo
    }

    if (addWNode(list,imageSurface->w,imageSurface->h,"ventana chida carnal"))
    {
        win1 = list->last;
        win1->name="win1";
        win1->iterateWindow=(win1it);
        
    }
    if (addWNode(list,imageSurface->w,imageSurface->h,"ventana2"))
    {
        win2 = list->last;
        win2->name="win2";
        win2->iterateWindow=(win1it);
        
    }
    else
    {
        return SDL_APP_FAILURE;
    }

    if (addWNode(list,imageSurface->w,imageSurface->h,"ventana3"))
    {
        win3 = list->last;
        win3->name="win3";
        win3->iterateWindow=(win3it);
    }
    else
    {
        return SDL_APP_FAILURE;
    }

    // Convertir la SDL_Surface a SDL_Texture para renderizarla

    imageTexture = SDL_CreateTextureFromSurface(win1->renderer, imageSurface);
    SDL_DestroySurface(imageSurface); // Ya no necesitamos la superficie
    if (!imageTexture)
    {
        SDL_Log("Error al crear textura: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_Texture * imageTexture2 = SDL_CreateTextureFromSurface(win2->renderer, copySurface);
    SDL_DestroySurface(copySurface); // Ya no necesitamos la superficie
    if (!imageTexture2)
    {
        SDL_Log("Error al crear textura: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderDrawColor(win1->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(win1->renderer);

    // Renderizar la imagen en la ventana 1
    SDL_RenderTexture(win1->renderer, imageTexture, NULL, NULL);
    SDL_RenderPresent(win1->renderer);

    SDL_SetRenderDrawColor(win2->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(win2->renderer);

    // Renderizar la imagen en la ventana 1
    
    SDL_RenderTexture(win2->renderer, imageTexture2, NULL, NULL);
    SDL_RenderPresent(win2->renderer);


    

    

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    //printf("event made %i \n",event->type);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    } else if (event->type == SDL_EVENT_CAMERA_DEVICE_APPROVED) {
        SDL_Log("Camera use approved by user!");
    } else if (event->type == SDL_EVENT_CAMERA_DEVICE_DENIED) {
        SDL_Log("Camera use denied by user!");
        //freeList(&list);
        return SDL_APP_FAILURE;
    }else if (event->type == SDL_EVENT_KEY_UP)
    {
        //freeList(&list);
        return SDL_APP_SUCCESS;
    }else if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
    {
        SDL_Window *closed_window = SDL_GetWindowFromID(event->window.windowID);
        

        winnode *rmnodewin = findNodeByWindow(&closed_window, list);
        rmwin(&rmnodewin, list);

        // Si ambas ventanas están cerradas, salir del programa
        if (list->count <= 0)
        {
            //freeList(&list);
            
            return SDL_APP_SUCCESS;
        }

    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
   
    winnode * current=list->head;
    while(current!=NULL){
        current->iterateWindow(current);
        current=current->next;

    }
    
    
    

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    freeList(&list);
    SDL_CloseCamera(camera);
    SDL_DestroyTexture(camtexture);
}

/*
    kaspersky
    nordpass
    bitwarden
    

*/
