#ifndef FGNG_DUMMY_H
#define FGNG_DUMMY_H

#define ORI_N 0
#define ORI_S 1
#define ORI_W 2
#define ORI_E 3

typedef struct fgng_dummy fgng_dummy;

struct fgng_dummy {
  SDL_Texture *texture;
  SDL_Texture *transitionTexture;
  SDL_Renderer **renderer;
  int xScale,yScale;
  int x,y;
  int sx,sy;
  int height;
  int width;
  int nOri;
  int ori;
  int anim;
  int nAnim;
  SDL_Rect pos;
  bool collided;
  int state;
  int transition;
  int activeTransitionAnim;
  bool overlapTransition;
  bool (*movement)(fgng_dummy *,SDL_Event *event);
};

bool fgng_dummy_runTransition(fgng_dummy *o,char *path,int nTrans,bool overlap);
bool fgng_dummy_changeTexture(fgng_dummy *o,char *path,int nAnim,int nOri);
bool fgng_dummy_frozenMovement(fgng_dummy *o,SDL_Event *event);
bool fgng_dummy_constantMovement(fgng_dummy *o,SDL_Event *event);
bool fgng_dummy_collision(fgng_dummy *o,fgng_dummy *target);
bool fgng_dummy_arrowManagement(fgng_dummy *o,SDL_Event *event);
bool fgng_dummy_changePosition(fgng_dummy *o);
bool fgng_dummy_renderCopy(fgng_dummy *o);
bool fgng_dummy_move(fgng_dummy *o,SDL_Event *event);
bool fgng_dummy_init(fgng_dummy *o,int x,int y,int sx,int sy,
		     int xScale,int yScale,
		     int height,int width,int nOri,int ori,SDL_Renderer **renderer,char *texture);
bool fgng_dummy_loadRenderer(fgng_dummy *o,SDL_Renderer **renderer);
bool fgng_dummy_loadTexture(fgng_dummy *o,char *path);
bool fgng_dummy_free(fgng_dummy *o);
bool fgng_dummy_arrowMovement(fgng_dummy *o,SDL_Event *event);
bool fgng_dummy_randomMovement(fgng_dummy *o,SDL_Event *event);
bool fgng_dummy_staticMovement(fgng_dummy *o,SDL_Event *event);

#endif
