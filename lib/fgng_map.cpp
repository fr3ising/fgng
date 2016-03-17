#include "fgng.h"
#include "fgng_map.h"
#include <stdlib.h>
#include <math.h>

bool fgng_map_init(fgng_map *o,int xScale,int yScale,
		   int height,int width,
		   SDL_Renderer **renderer,char *texture)
{
  Uint32 format;
  int access,w,h;
  o->xScale = xScale;
  o->yScale = yScale;
  o->width = width;
  o->height = height;
  fgng_map_loadRenderer(o,renderer);
  fgng_map_loadBaseTexture(o,texture);
  SDL_QueryTexture(o->baseTexture,&format,&access,&w,&h);
  return true;
}

bool fgng_map_loadRenderer(fgng_map *o,SDL_Renderer **renderer)
{
  o->renderer = renderer;
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
