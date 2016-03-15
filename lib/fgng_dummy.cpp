#include "fgng.h"
#include "fgng_dummy.h"
#include <stdlib.h>
#include <math.h>

const int SWIDTH = 640;
const int SHEIGHT = 480;

bool fgng_dummy_changeTexture(fgng_dummy *o,char *path,int nAnim,int nOri)
{
  Uint32 format;
  int access,w,h;
  if ( o->texture != NULL ) {
    SDL_DestroyTexture(o->texture);
  }
  fgng_dummy_loadTexture(o,path);
  SDL_QueryTexture(o->texture,&format,&access,&w,&h);
  if ( nAnim > 0 ) {
    o->nAnim = nAnim;
  }
  if ( nOri > 0 ) {
    o->nOri = nOri;
  }
  o->x = o->x*o->width*o->xScale;
  o->y = o->y*o->height*o->yScale;
  o->width = (int)w/o->nOri;
  o->height = (int)h/o->nAnim;
  o->x = o->x/(o->width*o->xScale);
  o->y = o->y/(o->height*o->yScale);
  return true;
}

bool fgng_dummy_init(fgng_dummy *o,int x,int y,int sx,int sy,
		     int xScale,int yScale,
		     int height,int width,int nOri,int ori,
		     SDL_Renderer **renderer,char *texture)
{
  Uint32 format;
  int access,w,h;
  o->transition = -1;
  o->state = 0;
  o->xScale = xScale;
  o->yScale = yScale;
  o->collided = false;
  o->x = x;
  o->y = y;
  o->sx = sx;
  o->sy = sy;
  o->nOri = nOri;
  o->ori = ori;
  o->width = width;
  o->height = height;
  fgng_dummy_loadRenderer(o,renderer);
  fgng_dummy_loadTexture(o,texture);
  SDL_QueryTexture(o->texture,&format,&access,&w,&h);
  o->anim = 0;
  o->nAnim = h/o->height;
  return true;
}

bool fgng_dummy_runTransition(fgng_dummy *o,char *path,int nTrans,bool overlap)
{
  SDL_Surface *surface;
  if ( o->transition > 0 ) {
    return false;
  }
  surface = IMG_Load(path);
  SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,0,0,0xFF));
  o->transitionTexture = SDL_CreateTextureFromSurface(*o->renderer,surface);
  SDL_FreeSurface(surface);
  o->transition = nTrans;
  o->activeTransitionAnim = nTrans;
  o->overlapTransition = overlap;
  return true;
}

bool fgng_dummy_renderCopy(fgng_dummy *o)
{
  SDL_Rect srcRect;
  Uint32 format;
  int access,w,h;
  o->pos.y = o->y*o->height;
  o->pos.x = o->x*o->width;
  o->pos.w = o->width*o->xScale;
  o->pos.h = o->height*o->yScale;
  if ( o->transition <= 0 ) { 
    srcRect.y = o->height*(o->anim % o->nAnim);
    if ( o->anim == o->nAnim ) {
      o->anim = 0;
    }
    srcRect.x = o->width*(o->ori % o->nOri);
    srcRect.w = o->width;
    srcRect.h = o->height;
    SDL_RenderCopy(*o->renderer,o->texture,&srcRect,&o->pos);
  } else {
    if ( o->overlapTransition ) {
      srcRect.y = o->height*(o->anim % o->nAnim);
      if ( o->anim == o->nAnim ) {
	o->anim = 0;
      }
      srcRect.x = o->width*(o->ori % o->nOri);
      srcRect.w = o->width;
      srcRect.h = o->height;
      SDL_RenderCopy(*o->renderer,o->texture,&srcRect,&o->pos);
    }
    SDL_QueryTexture(o->transitionTexture,&format,&access,&w,&h);
    h = h/o->activeTransitionAnim;
    srcRect.x = 0;
    srcRect.y = (o->activeTransitionAnim-o->transition)*h;
    srcRect.w = w;
    srcRect.h = h;
    SDL_RenderCopy(*o->renderer,o->transitionTexture,&srcRect,&o->pos);
    o->transition--;
    if ( o->transition <= 0 ) {
      SDL_DestroyTexture(o->transitionTexture);
    }
  }
  return true;
}

bool fgng_dummy_loadRenderer(fgng_dummy *o,SDL_Renderer **renderer)
{
  o->renderer = renderer;
  return true;
}

bool fgng_dummy_loadTexture(fgng_dummy *o,char *path)
{
  SDL_Surface *surface = IMG_Load(path);
  if ( surface == NULL ) {
    printf("Unable to load image %s! SDL_image Error: %s\n",path,IMG_GetError());
  } else {
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,0,0,0xFF));
    o->texture = SDL_CreateTextureFromSurface(*o->renderer,surface);
    if ( o->texture == NULL ) {
      printf("Unable to create texture from %s! SDL Error: %s\n",path,SDL_GetError());
    }
    SDL_FreeSurface(surface);
  }
  return true;
}

bool fgng_dummy_free(fgng_dummy *o)
{
  if ( o->texture != NULL ) {
    SDL_DestroyTexture(o->texture);
  }
  return true;
}

bool fgng_dummy_move(fgng_dummy *o,SDL_Event *event)
{
  return o->movement(o,event);
}

bool fgng_dummy_changePosition(fgng_dummy *o)
{
  o->x += o->sx;
  o->y += o->sy;
  return true;
}

bool fgng_dummy_arrowMovement(fgng_dummy *o,SDL_Event *event)
{
  if ( o->sx != 0 || o->sy != 0 ) {
    o->anim++;
  }
  if ( o->pos.x < 0 ) {
    o->x++;
    return false;
  }
  if ( o->pos.x+o->pos.w > SWIDTH ) {
    o->x--;
    return false;
  }
  if ( o->pos.y < 0 ) {
    o->y++;
    return false;
  }
  if ( o->pos.y+o->pos.h > SHEIGHT ) {
    o->y--;
    return false;
  }
  fgng_dummy_changePosition(o);
  return true;
}

bool fgng_dummy_arrowManagement(fgng_dummy *o,SDL_Event *event)
{
  if ( event->type == SDL_KEYDOWN ) {
    switch( event->key.keysym.sym ) {
    case SDLK_UP:
      o->ori = ORI_N;
      o->sy = -1;
      return true;
      break;
    case SDLK_DOWN:
      o->ori = ORI_S;
      o->sy = 1;
      return true;
      break;
    case SDLK_LEFT:
      o->ori = ORI_W;
      o->sx = -1;
      return true;
      break;
    case SDLK_RIGHT:
      o->ori = ORI_E;
      o->sx = 1;
      return true;
      break;
    }
  }
  if ( event->type == SDL_KEYUP ) {
    switch( event->key.keysym.sym ) {
    case SDLK_UP:
      o->anim = 0;
      o->sy = 0;
      return true;
      break;
    case SDLK_DOWN:
      o->anim = 0;
      o->sy = 0;
      return true;
      break;
    case SDLK_LEFT:
      o->anim = 0;
      o->sx = 0;
      return true;
      break;
    case SDLK_RIGHT:
      o->anim = 0;
      o->sx = 0;
      return true;
      break;
    }
  }
  return false;
}

bool fgng_dummy_randomMovement(fgng_dummy *o,SDL_Event *event)
{
  bool boundary = false;
  if ( o->pos.x < 0 ) {
    o->x++;
    o->sx *= -1;
    o->ori = ORI_E;
    boundary = true;
  }
  if ( o->pos.x+o->pos.w > SWIDTH ) {
    o->x--;
    o->sx *= -1;
    o->ori = ORI_W;
    boundary = true;
  }
  if ( o->pos.y < 0 ) {
    o->y++;
    o->sy *= -1;
    o->ori = ORI_S;
    boundary = true;
  }
  if ( o->pos.y+o->pos.h > SHEIGHT ) {
    o->y--;
    o->sy *= -1;
    o->ori = ORI_N;
    boundary = true;
  }
  if ( drand48() > 0.9 && !boundary ) {
    o->ori = rand() % 4;
  }
  if ( drand48() > 0.8 ) {
    o->anim++;
    switch( o->ori ) {
    case ORI_N:
      o->sx = 0;
      o->sy = -1;
      fgng_dummy_changePosition(o);
      break;
    case ORI_S:
      o->sx = 0;
      o->sy = 1;
      fgng_dummy_changePosition(o);
      break;
    case ORI_W:
      o->sx = -1;
      o->sy = 0;
      fgng_dummy_changePosition(o);
      break;
    case ORI_E:
      o->sx = 1;
      o->sy = 0;
      fgng_dummy_changePosition(o);
      break;
    }
  }
  return true;
}

bool fgng_dummy_staticMovement(fgng_dummy *o,SDL_Event *event)
{
  o->anim++;
  return true;
}

bool fgng_dummy_frozenMovement(fgng_dummy *o,SDL_Event *event)
{
  return true;
}

bool fgng_dummy_constantMovement(fgng_dummy *o,SDL_Event *event)
{
  o->anim++;
  fgng_dummy_changePosition(o);
  return true;
}

bool fgng_dummy_collision(fgng_dummy *o,fgng_dummy *target)
{
  int leftA,rightA,topA,bottomA;
  int leftB,rightB,topB,bottomB;
  leftA = o->pos.x;
  rightA = o->pos.x + o->pos.w;
  topA = o->pos.y;
  bottomA = o->pos.y + o->pos.h;
  leftB = target->pos.x;
  rightB = target->pos.x + target->pos.w;
  topB = target->pos.y;
  bottomB = target->pos.y + target->pos.h;
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
