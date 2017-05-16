#include <iostream>
#include <cmath>
#include "ioManager.h"
#include "bulletPool.h"
#include "SDL/SDL_rotozoom.h"
#include "frameFactory.h"

BulletPool::~BulletPool() {
  //std::cout << "bullerpool destructor" << std::endl;
  SDL_FreeSurface( bulletSurface );
  SDL_FreeSurface( invbulletSurface );
  //delete strategy;
  /*std::list<Bullet>::iterator ptr = bulletList.begin();
  while ( ptr != bulletList.end() ) {
    //delete (*ptr1)->getEnemy();
    (*ptr).deleteStrategy();
    ++ptr;
  }
  std::list<Bullet>::iterator ptr1 = freeList.begin();
  while ( ptr1 != freeList.end() ) {
    //delete (*ptr1)->getEnemy();
    (*ptr1).deleteStrategy();
    ++ptr1;
  }*/
}

BulletPool::BulletPool(const std::string& n) :
  name(n),
  strategy( new PerPixelCollisionStrategy ),
  bulletSurface(IOManager::getInstance().loadAndSet(
    Gamedata::getInstance().getXmlStr(name+"/file"), true)
  ),
  invbulletSurface(),
  bulletFrameLeft( FrameFactory::getInstance().getFrames(name+"reverse")),
  bulletFrameRight( FrameFactory::getInstance().getFrames(name)),
  //bulletFrameLeft(Gamedata::getInstance().getXmlInt(name+"reverse/frameInterval")),
  //bulletFrameRight(Gamedata::getInstance().getXmlInt(name+"/frameInterval")), 
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  direction( false ),
  bulletList(),
  freeList() 
{
  /*invbulletSurface = rotozoomSurface(bulletSurface, 180, 1, SMOOTHING_ON);
  unsigned num = Gamedata::getInstance().getXmlInt(name+"/frames"); 
  Uint32 pwidth = Gamedata::getInstance().getXmlInt(name+"/width");
  Uint32 pheight = Gamedata::getInstance().getXmlInt(name+"/height");
  Uint32 srcX = Gamedata::getInstance().getXmlInt(name+"/srcX");
  Uint32 srcY = Gamedata::getInstance().getXmlInt(name+"/srcY");
  for (unsigned i = 0; i < num; i++)
  {
    unsigned frameX = i*pwidth + srcX;   
    unsigned frameX1 = pwidth*(num-1-i) + srcX;   
    bulletFrameRight.push_back(new Frame(invbulletSurface, pwidth, pheight,frameX, srcY) );   
    bulletFrameLeft.push_back(new Frame(bulletSurface, pwidth, pheight,frameX1,srcX) ); 
  }*/
}

BulletPool::BulletPool(const BulletPool& b) :
  name(b.name),
  strategy(b.strategy),
  bulletSurface(b.bulletSurface),
  invbulletSurface(b.invbulletSurface),
  bulletFrameLeft(b.bulletFrameLeft),
  bulletFrameRight(b.bulletFrameRight), 
  frameInterval(b.frameInterval),
  timeSinceLastFrame(b.timeSinceLastFrame),
  direction(b.direction),
  bulletList(b.bulletList), 
  freeList(b.freeList)
{ }

bool BulletPool::collidedWith(const TwowayMultiSprite* obj){
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

bool BulletPool::shoot(const Vector2f& position, const Vector2f& velocity) {
  //std::cout << "!!!!!!!!!!In bullet pool shoot!!!!!!!!!!!!!!" << timeSinceLastFrame << ":" << frameInterval << std::endl;
  if (timeSinceLastFrame > frameInterval) {
     //std::cout << "in if check" << std::endl;
     //std::cout << freeList.empty() << std::endl;
    //std::list<Bullet>::iterator ptr = freeList.begin();
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      //std::cout << "freelist is empty, create new bullets" << std::endl;
      Bullet b(name, position, velocity*2.5, bulletFrameLeft,bulletFrameRight );
      b.setDir(direction);
      bulletList.push_back( b );
    }
    else {
      //std::cout << "freelist isnot empty" << std::endl;
      Bullet b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setDir(direction);
      b.setVelocity(velocity*2.5);
      b.setPosition(position);
      bulletList.push_back( b );
    }
    timeSinceLastFrame = 0;
    return true;
  }
  else
    return false;
}

void BulletPool::draw() const { 
  std::list<Bullet>::const_iterator ptr = bulletList.begin();
  //std::cout << "in bulletPool draw:" << bulletList.size() << std::endl;
  while (ptr != bulletList.end()) {
    /*if(ptr->getDir())
      std::cout << "current set dir is right" << std::endl;
    else
      std::cout << "current set dir is left" << std::endl;*/
    ptr->draw();
    ++ptr;
  }
}

void BulletPool::update(Uint32 ticks) { 
  //std::cout << "in bullet pool update, bulletList size is:" << bulletList.size() << std::endl;
  timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->setDir(direction);
    //std::cout << "bulletpool1 freelist size:" << freeList.size() << "bulletList size:" << bulletList.size() <<std::endl;
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      //std::cout << "gonetoofar line1, bullet list size" << bulletList.size() << std::endl;
      //std::cout << ptr->getDir() << std::endl;
      if(bulletList.size() > 0)
         ptr = bulletList.erase(ptr);
         //ptr = bulletList.erase(ptr);
      //std::cout << "gonetoofar line2" << std::endl;
    }  
    else ++ptr;
    //std::cout << "bulletpool2" << std::endl;
  }
}
