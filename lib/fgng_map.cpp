#include "fgng.h"
#include "fgng_map.h"
#include <stdlib.h>
#include <math.h>

bool fgng_map_init(fgng_map *o,int x,int y,
		   int nHor,int nVer,
		   int cw,int ch,
		   SDL_Renderer **renderer,char *texture)
{
  Uint32 format;
  int access;
  o->x = x;
  o->y = y;
  o->nHor = nHor;
  o->nVer = nVer;
  o->cam.x = x;
  o->cam.y = y;
  o->cam.w = cw;
  o->cam.h = ch;
  fgng_map_loadRenderer(o,renderer);
  fgng_map_loadBaseTexture(o,texture);
  SDL_QueryTexture(o->baseTexture,&format,&access,&o->w,&o->h);
  return true;
}

bool fgng_map_loadRenderer(fgng_map *o,SDL_Renderer **renderer)
{
  o->renderer = renderer;
  return true;
}

bool fgng_map_renderCopy(fgng_map *o)
{
  int i,j;
  SDL_Rect pos;
  pos.w = o->w;
  pos.h = o->h;
  //  printf("%d\t%d\n",o->cam.x,o->cam.y);
  printf("%d\t%d\n",o->cam.w,o->cam.h);

  for(i=0;i<o->nHor;i++) {
    pos.x = pos.w*i-o->cam.x;
    for(j=0;j<o->nVer;j++) {
      pos.y = pos.h*j-o->cam.y;
      //      if ( fgng_SDL_Rect_collision(&pos,&o->cam) ) {

	SDL_RenderCopy(*o->renderer,o->baseTexture,NULL,&pos);
	//      }
    }
  }
  return true;
}

bool fgng_map_loadBaseTexture(fgng_map *o,char *path)
{
  SDL_Surface *surface = IMG_Load(path);
  if ( surface == NULL ) {
    printf("Unable to load image %s! SDL_image Error: %s\n",path,IMG_GetError());
  } else {
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,0,0,0xFF));
    o->baseTexture = SDL_CreateTextureFromSurface(*o->renderer,surface);
    if ( o->baseTexture == NULL ) {
      printf("Unable to create texture from %s! SDL Error: %s\n",path,SDL_GetError());
    }
    SDL_FreeSurface(surface);
  }
  return true;
}

bool fgng_map_free(fgng_map *o)
{
  if ( o->baseTexture != NULL ) {
    SDL_DestroyTexture(o->baseTexture);
  }
  return true;
}
