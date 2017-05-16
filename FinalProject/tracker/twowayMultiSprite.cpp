#include "twowayMultiSprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "collisionStrategy.h"

TwowayMultiSprite:: ~TwowayMultiSprite(){
  //delete strategy;
}

void TwowayMultiSprite::deleteStrategy() {
  delete strategy;
}
TwowayMultiSprite::TwowayMultiSprite(const std::string& name) : 
  Drawable(name,
          Vector2f(Gamedata::getInstance().getXmlInt(name+"/srcX"), 
                    Gamedata::getInstance().getXmlInt(name+"/srcY")),  
          Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))),
  frames(FrameFactory::getInstance().getFrames(name)),
  framesLeft(FrameFactory::getInstance().getFrames(name+"/left")),
  framesRight(FrameFactory::getInstance().getFrames(name+"/right")),
  framesUp(FrameFactory::getInstance().getFrames(name+"/right")),
  framesDown(FrameFactory::getInstance().getFrames(name+"/right")),
  strategy(new PerPixelCollisionStrategy),
  spritename(name),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame(0),
  reverse(false),
  left(false),
  right(false),
  up(false),
  down(false),
  stop(false),
  smartenemyrev(false)
{ }

TwowayMultiSprite::TwowayMultiSprite(const TwowayMultiSprite &s) : 
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  frames(s.frames),
  framesLeft(s.framesLeft),
  framesRight(s.framesRight),
  framesUp(s.framesUp),
  framesDown(s.framesDown),
  strategy(s.strategy),
  spritename(s.spritename),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  reverse(s.reverse),
  left(s.left),
  right(s.right),
  up(s.up),
  down(s.down),
  stop(s.stop),
  smartenemyrev(s.smartenemyrev)
{ }

TwowayMultiSprite::TwowayMultiSprite( const std::string& name,  const Vector2f& pos, const Vector2f& vel, 
                  std::vector<Frame*> & fmsLeft,
                  std::vector<Frame*> & fmsRight) :
  Drawable(name,pos,vel),
  frames(fmsRight),
  framesLeft(fmsLeft),
  framesRight(fmsRight),
  framesUp(fmsRight),
  framesDown(fmsRight),
  strategy(new PerPixelCollisionStrategy),
  spritename(name),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  reverse(false),
  left(false),
  right(false),
  up(false),
  down(false),
  stop(false),
  smartenemyrev(false)
{ }

TwowayMultiSprite::TwowayMultiSprite(const std::string& name, float x, float y, float sX, float sY) :
  Drawable(name, Vector2f(x,y), Vector2f(sX,sY)),
  frames(FrameFactory::getInstance().getFrames(name)),
  framesLeft(FrameFactory::getInstance().getFrames(name+"/left")),
  framesRight(FrameFactory::getInstance().getFrames(name+"/right")),
  framesUp(FrameFactory::getInstance().getFrames(name+"/right")),
  framesDown(FrameFactory::getInstance().getFrames(name+"/right")),
  strategy(new PerPixelCollisionStrategy),
  spritename(name),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  reverse(false),
  left(false),
  right(false),
  up(false),
  down(false),
  stop(false),
  smartenemyrev(false)
{ }

void TwowayMultiSprite::draw() const { 
  //std::cout << "reverse flag:" << this.getReverse() << std::endl;
  //std::cout << "right flag:" << this.getRight() << std::endl;
  //std::cout << "in 2 way multisprite draw" << std::endl;
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void TwowayMultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  //Vector2f currVelocity = getVelocity();
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
  }
  /*if(currVelocity[0] >= 0)
    currentFrame = 0;
  else
    currentFrame += 1;*/
  /*  if (timeSinceLastFrame > frameInterval) {
      timeSinceLastFrame = 0;
      Vector2f currVelocity = getVelocity();
      if ( currVelocity[0] == 0 )
         currentFrame = 1;
      if ( currVelocity[0] > 0 ) {
         if ( currentFrame < ((numberOfFrames/2)-1) )
            currentFrame += 1; 
         else
             currentFrame = 0;
      }
      if ( currVelocity[0] < 0 ) {
         if ( currentFrame <= (numberOfFrames-1) && currentFrame > ((numberOfFrames/2)) )
            currentFrame -=1;
         else
            currentFrame = (numberOfFrames-1);
      }
  }*/
}

void TwowayMultiSprite::update(Uint32 ticks) {
  //std::cout << "in 2 way multisprite update, player name is:" << getName() <<std::endl;
  if( getName() == "player") {
    //std::cout << "player is player" << std::endl;
    Vector2f moveY = Vector2f(0, 10);
    Vector2f moveX = Vector2f(10, 0);
  
  if ( Y() <= 0) {
    setPosition( getPosition() + moveY );
  }
  if ( Y() > worldHeight-frameHeight) {
    setPosition( getPosition() - moveY );
  }
  if ( X() < 0) {
    setPosition( getPosition() + moveX );
    frames = framesLeft;
  }
  if ( X() > worldWidth-frameWidth) {
    setPosition( getPosition() - moveX );
    frames = framesRight;
  }

  if(getUp()) {
    if(getRight()) {
      //std::cout << "Up and right flag set" << std::endl;
      numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/right/frames");
      frames=framesRight;
    }
    else if(getLeft()) {
      //std::cout << "Up and left flag set" << std::endl;
      numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/left/frames");
      frames=framesLeft;
    }
    else {
      //std::cout << "Up flag set" << std::endl;
      numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/right/frames");
      frames = framesUp;
    }
  }
  else if(getDown()) {
    if(getRight()) {
      //std::cout << "Down and right flag set" << std::endl;
      numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/right/frames");
      frames=framesRight;
    }
    else if(getLeft()) {
      //std::cout << "Down and left flag set" << std::endl;
      numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/left/frames");
      frames=framesLeft;
    }
    else {
      //std::cout << "Down flag set" << std::endl;
      numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/down/frames");
      frames=framesDown;
    }
  }
  else if(getRight()) {
    //std::cout << "right flag set" << std::endl;
    numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/right/frames");
    frames=framesRight;
  }
  else if(getLeft()){
    //std::cout << "left flag set" << std::endl;
    numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/left/frames");
    frames=framesLeft;
  }
  /*if(getLeft()) {
    //std::cout << "Left flag set" << std::endl;
    numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/left/frames");
    frames=framesLeft;
  }
  if(getRight()) {
    //std::cout << "Right flag set" << std::endl;
    numberOfFrames = Gamedata::getInstance().getXmlInt(getName()+"/right/frames");
    frames=framesRight;
  }*/
  //std::cout << "reverse flag:" << getLeft() << "right flag:" << getRight() << getName() << std::endl;
  if(!getStop()) 
    advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
 }
 if( getName() == "dragon" || getName() == "greendragon") {
  //std::cout << "player is dragon" << std::endl;
  advanceFrame(ticks);
  //Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
    frames = framesLeft;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    frames = framesRight;
  }
  if( getName() == "greendragon" ) {
   //std::cout << "greendragon smart flag" << smartenemyrev << std::endl;
   //std::cout << "incr:" << incr << std::endl;
   if(smartenemyrev) {
     //std::cout << "greendragon smart flag" << smartenemyrev << "incr is:" << incr << std::endl;
     velocityX( abs( velocityX() ) );
     frames = framesLeft;
     smartenemyrev=false;
   } 
  }
 }
 /*if( getName() == "greendragon") {
   std::cout << "player is green dragon" << std::endl;
 }*/
  //std::cout << "in 2way frame update, velocity and position modified" << std::endl;
}

bool TwowayMultiSprite::collidedWith(const TwowayMultiSprite * obj) const{
  return strategy->execute(*this, *obj);
}
