#ifndef FGNG_MAP_H
#define FGNG_MAP_H

typedef struct fgng_map fgng_map;

struct fgng_map {
  SDL_Texture *baseTexture;
  SDL_Renderer **renderer;
  SDL_Rect cam;
  int nHor;
  int nVer;
  int w,h;
  int x,y;
};

bool fgng_map_init(fgng_map *o,int x,int y,
		   int nHor,int nVer,
		   int cw,int ch,
		   SDL_Renderer **renderer,char *texture);
bool fgng_map_loadRenderer(fgng_map *o,SDL_Renderer **renderer);
bool fgng_map_loadBaseTexture(fgng_map *o,char *path);
bool fgng_map_free(fgng_map *o);
bool fgng_map_renderCopy(fgng_map *o);

#endif
