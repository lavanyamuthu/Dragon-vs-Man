#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include "twowayMultiSprite.h"
#include "bulletPool.h"
//#include "collisionStrategy.h"

class Player {
public:
  Player(const std::string& name);
  Player(const Player&);
  ~Player();

  void update(Uint32 ticks);
  void draw() const;
  //const Drawable* getSprite() const { return drawable; }
  //const Drawable* getProj() const { return proj->getSprite(); }
  const Drawable* getDrawable() const {
    return (&playerSprite);
  }
  bool collidedWith(const TwowayMultiSprite* obj);
  bool bulletHitEnemy(const TwowayMultiSprite* obj);
  /*bool collidedWith(const MultiSprite* d) const {
    return strategy->execute(playerSprite, *d);
  }*/
  /*void setCollisionStrategy(int index) {
    strategy = strategies[index];
  }*/
  float getX() const { return playerSprite.Drawable::X(); }
  float getY() const { return playerSprite.Drawable::Y(); }
  //const std::string& getNameofPlayer() const { return playerSprite.Drawable::getName(); }
  void setX(float a) {
    playerSprite.Drawable::X(a);
  }
  void setY(float b) {
    playerSprite.Drawable::Y(b);
  }
  void setvelX(int a) {
    velX = a;
  }
  void setvelY(int b) {
    velY = b;
  }
  bool shooting();
  bool checkCollisions();
  void moveLeft();
  void moveRight();
  void jump();
  void setplayDir(int flag) {
    playDir = flag;
  }
  void setinitY(int val) {
    initY = val;
  }
  int getinitY() const {
    return initY;
  }
  void setplayerJump(bool flag) {
    jumpFlag = flag;
  }
  bool getplayerJump() const {
    return jumpFlag;
  }
  bool getplayerReverse() const {
    return reverse;
  }
  bool getDead() const {
    return dead;
  }
  void setDead(bool flag) {
    dead = flag;
  }
  void setreassemble (bool flag) {
    reassemble = flag;
  }
  bool getreassemble() const {
    return reassemble;
  }
  void setShoot(bool flag) {
    shoot = flag;
  }
  bool getShoot() {
    return shoot;
  }
  bool getReverse() const {
    return reverse;
  }
  void setReverse(bool flag) {
    reverse = flag;
  }
  unsigned int getBulletListCount() const {
    return bullets.bulletCount();
  }
  unsigned int getFreeListCount() const {
    return bullets.freeCount();
  }
  bool getGod() const {
    return god;
  }
  void setGod(bool flag) {
    god = flag;
  }

private:
  bool xDirection;
  bool yDirection;
  bool reverse;
  bool jumpFlag;
  int playDir;
  int velX;
  int velY;
  int initY;
  bool dead;
  bool reassemble;
  bool shoot;
  bool god;
  
  TwowayMultiSprite playerSprite;
  //BulletPool *fire;
  std::string bulletName;
  BulletPool bullets;
  float bulletSpeed;
  Vector2f incrForwardPosition;
  Vector2f incrReversePosition;
  CollisionStrategy * strategy;

  Player();
  //Player(const Player&);
  Player& operator=(const Player&);
};
#endif
