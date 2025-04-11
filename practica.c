

#include <SDL3/SDL.h>
#include <stdio.h>
#include "practica.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void rmwin(winnode **nodemain, winlist *list)
{
    //elimina 2 despues 3 y despues 1 genera error
    //
    #ifdef DEBUGMUR
    winnode *aux=list->head;
    while(aux!=NULL){
        printf(aux->name);
        if (aux->next==NULL)
        {
            printf("(next = NULL)");
        }else{
            printf("(next = %s)",aux->next->name);
        }
        if (aux==list->head)
        {
            printf("(prev = NULL)");
        }else{
            printf("(prev = %s)",aux->prev->name);
        }
        
        printf(" -> ");
        aux=aux->next;
    }
    
    printf(" \n ");
    #endif
    winnode *node = *nodemain;
    SDL_DestroyRenderer(node->renderer);
    SDL_DestroyWindow(node->window);
    node->renderer = NULL;
    node->window = NULL;
    
    if (node != list->head)
    {
        
        node->prev->next = node->next;
        
        if (node->next!= NULL)
        {
            node->next->prev=node->prev;
        }
            
        
        printf("se libero un nodo que no era la cabeza\n");
    }
    else if (node == list->head)
    {
        list->head = node->next;
        printf("se libero la cabeza\n");
    }
    list->count--;
    SDL_free(node);
    *nodemain=NULL;
    #ifdef DEBUGMUR
    aux=list->head;
    while(aux!=NULL){
        printf(aux->name);
        if (aux->next==NULL)
        {
            printf("(next = NULL)");
        }else{
            printf("(next = %s)",aux->next->name);
        }
        if (aux==list->head)
        {
            printf("(prev = NULL)");
        }else{
            printf("(prev = %s)",aux->prev->name);
        }
        
        
        printf(" -> ");
        
        aux=aux->next;
    }
    
    printf(" \n count=%i\n",list->count);
    #endif
    
    
}

winnode *findNodeByWindow(SDL_Window **windowlmain, winlist *list)
{
    SDL_Window *windowl = *windowlmain;
    winnode *aux = list->head;

    while (aux != NULL)
    {
        if (aux->window == windowl)
        {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

void freeList(winlist **listMain)
{
    winlist *list = *listMain;
    winnode *node = list->head;
    while (node != NULL)
    {

        list->head = node->next;
        SDL_DestroyRenderer(node->renderer);
        SDL_DestroyWindow(node->window);
        node->renderer = NULL;
        node->window = NULL;

        //rmwin(&node,list);
        SDL_free(node);
        
        node = list->head;
    }
    SDL_free(list);
    list=NULL;
}

bool initWinlist(winlist **list)
{
    *list = (winlist *)SDL_malloc(sizeof(winlist));
    if ((*list) == NULL)
    {
        SDL_Log("no se pudo crear la lista de ventanas, %s", SDL_GetError());
        return false;
    }
    (*list)->head = NULL;
    (*list)->count = 0;

    

    
    (*list)->last = NULL;
    (*list)->count = 0;

    

    

    return true;
}

bool addWNode(winlist *list,int with,int hight,const char * title)
{

    winnode *nnode = (winnode *)SDL_malloc(sizeof(winnode));
    if (nnode == NULL)
    {
        SDL_Log("no se pudo crear la nueva ventana, %s", SDL_GetError());

        return false;
    }

    if (list->head == NULL)
    {

        list->head = nnode;
        list->last = nnode;
    }
    else if (list->head == list->last)
    {
        list->head->next = nnode;
        nnode->prev = list->head;
        list->last = nnode;
    }
    else
    {
        list->last->next = nnode;
        nnode->prev = list->last;
        list->last = nnode;
    }

    list->count++;

    if (!SDL_CreateWindowAndRenderer(title, with, hight, 0, &(nnode->window), &(nnode->renderer)))
        {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return false;
        }
        nnode->iterateWindow=NULL;
    
    return true;
}


