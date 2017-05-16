#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include <list>
#include<vector>
#include "bullet.h"
#include "collisionStrategy.h"

class BulletPool {
public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&);
  ~BulletPool();
  void draw() const;
  void update(Uint32 ticks);
  bool shoot(const Vector2f& pos, const Vector2f& vel);
  bool getPlayerDir() { return direction; }
  void setPlayerDir(bool flag) { direction = flag; }
  bool collidedWith(const TwowayMultiSprite* obj);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool shooting() const { return bulletList.empty(); }
  //bool collidedWith(const Drawable* obj) const;
private:
  std::string name;
  CollisionStrategy* strategy;
  SDL_Surface *bulletSurface;  
  SDL_Surface *invbulletSurface;

  std::vector<Frame*> bulletFrameLeft;
  std::vector<Frame*> bulletFrameRight;
  
  float frameInterval;
  float timeSinceLastFrame;
  bool direction;
  std::list<Bullet> bulletList; // Bullets is a list of Bullet
  std::list<Bullet> freeList;   // when a Bullet is out of range

  BulletPool& operator=(const BulletPool&);   // (Item 6)
};
#endif
