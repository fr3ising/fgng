#include "fgng.h"
#include <SDL.h>

bool fgng_init(SDL_Window **window,SDL_Renderer **renderer,char *title,int sw,int sh)
{
  int imgFlags;
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    return false;
  } else {
    *window = SDL_CreateWindow(title, 
			      SDL_WINDOWPOS_UNDEFINED, 
			      SDL_WINDOWPOS_UNDEFINED, 
			      sw,sh, 
			      SDL_WINDOW_SHOWN );
    if( *window == NULL ) {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
      return false;
    } else {
      *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if ( *renderer == NULL ) {
	printf("Renderer could not be created! SDL Error: %s\n",SDL_GetError());
	return false;
      } else {
	printf("%p\n",(void *)renderer);
	SDL_SetRenderDrawColor(*renderer,0xFF,0xFF,0xFF,0xFF);
	imgFlags = IMG_INIT_PNG;
	if ( !(IMG_Init(imgFlags) & imgFlags ) ) {
	  printf("SDL_image could not initialize! SDL_Image Error: %s\n",IMG_GetError());
	  return false;
	}
	if( Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048 ) < 0 ) {
	  printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	  return false;
	}
	if( TTF_Init() == -1 ) {
	  printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	  return false;
	}
      }
    }
  }
  return true;
}

bool fgng_close(SDL_Window **window,SDL_Renderer **renderer)
{
  SDL_DestroyRenderer(*renderer);
  SDL_DestroyWindow(*window);
  IMG_Quit();
  SDL_Quit();
  return true;
}

bool fgng_SDL_Rect_collision(SDL_Rect *o,SDL_Rect *target)
{
  int leftA,rightA,topA,bottomA;
  int leftB,rightB,topB,bottomB;
  leftA = o->x;
  rightA = o->x + o->w;
  topA = o->y;
  bottomA = o->y + o->h;
  leftB = target->x;
  rightB = target->x + target->w;
  topB = target->y;
  bottomB = target->y + target->h;
  if( bottomA <= topB ) {
    return false;
  }
  if( topA >= bottomB ) {
    return false;
  }
  if( rightA <= leftB ) {
    return false;
  }
  if( leftA >= rightB ) {
    return false;
  }
  return true;

}
