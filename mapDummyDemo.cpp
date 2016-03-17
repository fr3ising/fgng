#include <fgng.h>
#include <fgng_dummy.h>
#include <fgng_map.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

const int SWIDTH = 640;
const int SHEIGHT = 480;
#define SPEED 60
#define DEALERS 24

fgng_dummy link;
fgng_dummy wizard[DEALERS];

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *bgTexture = NULL;
SDL_Texture *heart = NULL;
SDL_Texture *scoreTexture;
SDL_Texture *introTexture;
SDL_Color textColor = { 0, 255, 255 };
TTF_Font *font;
SDL_Event event;
int score = 0;

Mix_Chunk *out;
Mix_Chunk *burning;
Mix_Chunk *death;
Mix_Chunk *hurt;
Mix_Chunk *hit;
Mix_Chunk *requiem;

int showScore(void)
{
  char msg[256];
  SDL_Surface *textSurface;
  SDL_Rect pos;
  sprintf(msg,"Score %d",score);
  textSurface = TTF_RenderText_Solid(font,msg, textColor );
  scoreTexture = SDL_CreateTextureFromSurface(renderer,textSurface);
  SDL_FreeSurface(textSurface);
  pos.x = 470;
  pos.y = 20;
  pos.h = 30;
  pos.w = strlen(msg)*20;
  SDL_RenderCopy(renderer,scoreTexture,NULL,&pos);
  SDL_DestroyTexture(scoreTexture);
  return 0;
}

int initDummies(void)
{
  int i;
  fgng_dummy_init(&link,10,10,0,0,3,3,24,16,4,0,&renderer,(char *)"./data/images/chars/link1.png");
  link.movement = &fgng_dummy_arrowMovement;
  link.state = 4;
  for (i=0;i<DEALERS;i++) {
    fgng_dummy_init(&wizard[i],(int)(drand48()*(SWIDTH-16)/16),(int)(drand48()*(SHEIGHT-16)/16),
		    0,0,3,3,16,16,4,0,&renderer,(char *)"./data/images/chars/wizard1.png");
    wizard[i].movement = &fgng_dummy_randomMovement;
    wizard[i].collided = false;
    wizard[i].state = 0;
  }
  return 0;
}

int initSounds(void)
{
  burning = Mix_LoadWAV("./data/sounds/fire.wav");
  out = Mix_LoadWAV("./data/sounds/out.wav");
  death = Mix_LoadWAV("./data/sounds/death.wav");
  hit = Mix_LoadWAV("./data/sounds/hit.wav");
  requiem = Mix_LoadWAV("./data/sounds/requiem.wav");
  hurt = Mix_LoadWAV("./data/sounds/hurt.wav");
  return 0;
}

int renderDummies(void) {
  int i;
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer,bgTexture,NULL,NULL);
  fgng_dummy_move(&link,&event);
  for(i=0;i<DEALERS;i++) {
    fgng_dummy_move(&wizard[i],&event);
  }
  for(i=0;i<DEALERS;i++) {
    fgng_dummy_renderCopy(&wizard[i]);
  }
  fgng_dummy_renderCopy(&link);
  for(i=0;i<DEALERS;i++) {
    if ( (wizard[i].pos.y+wizard[i].pos.h) > (link.pos.y+link.pos.h) ) {
      if ( fgng_dummy_collision(&link,&wizard[i]) ) {
	fgng_dummy_renderCopy(&wizard[i]);
      }
    }
  }
  return 0;
}

int renderHearts()
{
  int i;
  SDL_Rect pos;
  for(i=0;i<link.state+1;i++) {
    pos.x = 10+i*20;
    pos.y = 20;
    pos.w = 30;
    pos.h = 30;
    SDL_RenderCopy(renderer,heart,NULL,&pos);
  }
  return 0;
}

int playOutSounds(void)
{
  int i;
  for(i=0;i<DEALERS;i++) {
    if ( wizard[i].state != 2 && (wizard[i].pos.x-wizard[i].pos.w > SWIDTH) ) {
      wizard[i].state = 2;
      score++;
      Mix_PlayChannel(-1,out,0);
    }
    if ( wizard[i].state != 2 && (wizard[i].pos.y-wizard[i].pos.h > SHEIGHT) ) {
      wizard[i].state = 2;
      score++;
      Mix_PlayChannel(-1,out,0);
    }
    if ( wizard[i].state != 2 && (wizard[i].pos.y+wizard[i].pos.h < 0) ) {
      wizard[i].state = 2;
      score++;
      Mix_PlayChannel(-1,out,0);
    }
    if ( wizard[i].state != 2 && (wizard[i].pos.x+wizard[i].pos.w < 0) ) {
      wizard[i].state = 2;
      score++;
      Mix_PlayChannel(-1,out,0);
    }
  }
  return 0;
}

int computeCollisions(void)
{
  int i;
  for(i=0;i<DEALERS;i++) {
    if ( wizard[i].collided == false ) {
      if ( fgng_dummy_collision(&link,&wizard[i]) ) {
	if ( link.sx != 0 || link.sy != 0 ) {
	  if ( wizard[i].state == 1 ) {
	    if ( fgng_dummy_runTransition(&link,(char *)"./data/images/transitions/fire.png",3,true) ) {
	      link.state--;
	      Mix_PlayChannel(-1,hurt,0);
	    }
	    Mix_PlayChannel(-1,burning,0);
	  } 
	  Mix_PlayChannel(-1,hit,0);
	  wizard[i].collided = true;
	  wizard[i].sx = 2.0*link.sx;
	  wizard[i].sy = 2.0*link.sy;
	  wizard[i].movement = &fgng_dummy_constantMovement;
	} 
      }
    }
  }
  return 0;
}  

int computeCollisions2(void)
{
  int i,j;
  for(i=0;i<DEALERS;i++) {
    if ( wizard[i].collided == true ) {
      for(j=0;j<DEALERS;j++) {
	if ( i == j ) {
	  continue;
	}
	if ( !wizard[j].collided && fgng_dummy_collision(&wizard[i],&wizard[j]) ) {
	  if ( wizard[j].state == 1 ) {
	    fgng_dummy_runTransition(&wizard[i],(char *)"./data/images/transitions/fire.png",3,true);
	    Mix_PlayChannel(-1,burning,0);
	    wizard[i].state = 1;
	    fgng_dummy_changeTexture(&wizard[i],(char *)"./data/images/chars/red1.png",0,0);
	  }
	  wizard[i].collided = false;
	  wizard[i].sx = 0;
	  wizard[i].sy = 0;
	  wizard[i].movement = &fgng_dummy_randomMovement;
	  fgng_dummy_runTransition(&wizard[j],(char *)"./data/images/transitions/fire.png",3,true);
	  Mix_PlayChannel(-1,burning,0);
	  wizard[j].state = 1;
	  fgng_dummy_changeTexture(&wizard[j],(char *)"./data/images/chars/red1.png",0,0);
	}
      }
    }
  }
  return 0;
}

int main(int argc,char *argv[])
{
  int i;
  bool quit;
  bool start;
  bool death;
  SDL_Surface *bgSurface = NULL;
  SDL_Surface *heartSurface = NULL;

  fgng_init(&window,&renderer,(char *)"dummy Demo",SWIDTH,SHEIGHT);

  font = TTF_OpenFont( "./data/font/font.ttf", 28 );
  bgSurface = IMG_Load((char *)"./data/images/backgrounds/background.png");
  bgTexture = SDL_CreateTextureFromSurface(renderer,bgSurface);
  SDL_FreeSurface(bgSurface);

  heartSurface = IMG_Load((char *)"./data/images/icons/heart.png");
  SDL_SetColorKey(heartSurface,SDL_TRUE,SDL_MapRGB(heartSurface->format,0,0,0xFF));
  heart = SDL_CreateTextureFromSurface(renderer,heartSurface);
  SDL_FreeSurface(heartSurface);
  
  initSounds();
  initDummies();

  quit = false;
  start = false;
  death = false;

  while ( !quit ) {
    SDL_Delay(SPEED);
    while ( SDL_PollEvent(&event) != 0 ) {
      if ( event.type == SDL_QUIT ) {
	quit = true;
      } else {
	if ( event.type == SDL_KEYDOWN ) {
	  start = true;
	}
	if ( start == true ) {
	  if ( ! death ) {
	    fgng_dummy_arrowManagement(&link,&event);
	  }
	}
      }
    }
    renderDummies();
    renderHearts();
    showScore();
    if ( link.state < 0 ) {
      Mix_PlayChannel(-1,requiem,0);
      link.movement = &fgng_dummy_frozenMovement;
      fgng_dummy_runTransition(&link,(char *)"./data/images/transitions/fire.png",3,true);
      fgng_dummy_runTransition(&link,(char *)"./data/images/transitions/fire.png",3,true);
      fgng_dummy_runTransition(&link,(char *)"./data/images/transitions/fire.png",3,true);
      fgng_dummy_runTransition(&link,(char *)"./data/images/transitions/fire.png",3,true);
      for(i=0;i<DEALERS;i++) {
	wizard[i].sx *= 3;
	wizard[i].sy *= 3;
	fgng_dummy_runTransition(&wizard[i],(char *)"./data/images/transitions/fire.png",3,true);
      }
      link.sx = 0;
      link.sy = 0;
      death = true;
    }
    SDL_RenderPresent(renderer);
    playOutSounds();
    computeCollisions();
    computeCollisions2();
  }

  fgng_dummy_free(&link);
  for(i=0;i<DEALERS;i++) {
    fgng_dummy_free(&wizard[i]);
  }
  
  fgng_close(&window,&renderer);
  return 0;
}
