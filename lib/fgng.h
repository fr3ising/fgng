#ifndef FGNG_H
#define FGNG_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>

#define OX 0
#define OY 1
#define OZ 2

#define TRUE 1
#define FALSE 0

bool fgng_init(SDL_Window **window,SDL_Renderer **renderer,char *title,int sw,int sh);
bool fgng_close(SDL_Window **window,SDL_Renderer **renderer);

#endif
