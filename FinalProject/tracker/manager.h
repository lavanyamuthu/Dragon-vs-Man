#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "enemy.h"
#include "explodingSprite.h"
#include "scaledSprite.h"
#include "hud.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void reset();
  /*void setDrawnFlag(bool flag) {
    eggdrawn = flag;
  }*/

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World world1;
  World world2;
  World world3;
  World world4;
  //World world5;
  Viewport& viewport;

  SDL_Surface * const orbSurface;
  //SDL_Surface * rockSurface;
  //const Frame * const rockFrame;
  //const Frame* const eggFrame;
  //SDL_Surface* starSurface;
  //std::vector<Frame*> starFrames;
  //std::vector<SDL_Surface*> surfaces;

  std::vector<Enemy*> dragons;
  //std::vector<Drawable*> sprites;
  std::list<ExplodingSprite*> explodingSprites;
  std::list<Drawable*>::iterator currentSprite;
  std::vector<ScaledSprite*> sprites;
  Sprite egg;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  int enemyCount;
  int enemyFaceCount;
  bool win;
  bool entry;
  bool eggdrawn;
  Player player;
  //std::list<Enemy*> enemy;
  //Hud hud;
  Hud hudHelp;
  Hud hudCount;
  SDLSound sound;
  //Hud playerHealth;
  //Enemy smartenemy;

  void draw() const;
  void update();
  //void makeSprites();
  //bool checkForCollisions() const;

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
  void makeOrbs();
  void printOrbs() const;
};
