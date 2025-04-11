#ifndef GRAFICAS_CORE_H
#define GRAFICAS_CORE_H

#include <SDL3/SDL.h>
#include <stdbool.h>
typedef struct winnode
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    winnode *prev;
    winnode *next;
    const char* name;
    void (*iterateWindow)( winnode *);

} winnode;

typedef struct winlist
{
    int count;
    winnode *head;
    winnode *last;
} winlist;
bool addWNode(winlist *list,int with,int hight,const char * title);

void rmwin(winnode **nodemain, winlist *list);
winnode *findNodeByWindow(SDL_Window **windowlmain, winlist *list);
void freeList(winlist **listMain);
bool initWinlist(winlist **list);
#endif
