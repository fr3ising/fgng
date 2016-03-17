#ifndef FGNG_MAP_H
#define FGNG_MAP_H

typedef struct fgng_map fgng_map;

struct fgng_map {
  SDL_Texture *baseTexture;
  SDL_Renderer **renderer;
  int xScale,yScale;
  int height;
  int width;
};

bool fgng_map_init(fgng_map *o,int xScale,int yScale,
		   int height,int width,SDL_Renderer **renderer,char *texture);
bool fgng_map_loadRenderer(fgng_map *o,SDL_Renderer **renderer);
bool fgng_map_loadBaseTexture(fgng_map *o,char *path);
bool fgng_map_free(fgng_map *o);

#endif
