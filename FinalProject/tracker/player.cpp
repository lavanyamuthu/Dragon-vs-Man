#include "player.h"
#include "gamedata.h"

Player::~Player() {
  //std::cout << "in player destructor" << std::endl;
  delete strategy;
  //playerSprite.deleteStrategy();
}

Player::Player(const std::string& name) :
  xDirection(false),
  yDirection(false),
  reverse(false),
  jumpFlag(false),
  playDir(0),
  velX(Gamedata::getInstance().getXmlInt(name+"/speedX")),
  velY(Gamedata::getInstance().getXmlInt(name+"/speedY")),
  initY(0),
  dead(false),
  reassemble(false),
  shoot(true),
  god(false),
  playerSprite(name),
  bulletName(Gamedata::getInstance().getXmlStr("bulletName")),
  bullets(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  incrForwardPosition(Gamedata::getInstance().getXmlFloat(bulletName+"/forwardPositionX"),
                                                         Gamedata::getInstance().getXmlFloat(bulletName+"/forwardPositionY")),
  incrReversePosition(Gamedata::getInstance().getXmlFloat(bulletName+"/reversePositionX"),
                                                         Gamedata::getInstance().getXmlFloat(bulletName+"/reversePositionY")),
  strategy(new PerPixelCollisionStrategy)
{ }

Player::Player(const Player &p) :
  xDirection( p.xDirection ),
  yDirection( p.yDirection ),
  reverse( p.reverse ),
  jumpFlag( p.jumpFlag ),
  playDir( p.playDir ),
  velX( p.velX ),
  velY( p.velY ),
  initY( p.initY ),
  dead( p.dead ),
  reassemble( p.reassemble ),
  shoot( p.shoot ),
  god( p.god ),
  playerSprite( p.playerSprite ),
  bulletName(  p.bulletName ),
  bullets( p.bullets ),
  bulletSpeed( p.bulletSpeed ),
  incrForwardPosition( p.incrForwardPosition ),
  incrReversePosition( p.incrReversePosition ),
  strategy( p.strategy )
{ }

void Player::draw() const {
  //std::cout << "in player draw and shoot flag is:" << shoot << std::endl;
  //std::cout << "in player draw, dead flag: " << dead << "reassemble flag: " << reassemble << std::endl;
  if(!dead) {
    //std::cout << "in player draw, dead flag is: " << dead << " and shoot flag is: " << shoot << std::endl;
    playerSprite.draw();
    if(shoot)
      bullets.draw();
    return;
  }
  if( dead && reassemble ) {
    //std::cout << " in player draw reassemble" << std::endl;
    playerSprite.draw();
  }
}

/* 0 - stop, left and right or up and down or key up
   1 - right
   2 - left
   3 - up
   4 - down
   5 - left and up
   6 - left and down
   7 - right and up
   8 - right and down
   9 - shoot
*/
void Player::update(Uint32 ticks) {
 //std::cout << "in player update and dir flag is:" << playDir << std::endl;
 if(!dead) {
    bullets.update(ticks);
    //std::cout << "in player update1" << std::endl;
    if(playDir == 0) {
      //std::cout << "In player stop" << std::endl;
      /*if( playerSprite.Drawable::Y() <= 0) {
        playerSprite.Drawable::Y(0);
        playerSprite.Drawable::velocityY(0);
      }
      if( playerSprite.Drawable::Y() >= playerSprite.getWorldHeight() - playerSprite.getFrameWidth() ) {
        //playerSprite.Drawable::Y(0);
        playerSprite.Drawable::velocityY( playerSprite.getWorldHeight() - playerSprite.getFrameWidth() );
      }
      playerSprite.Drawable::X(playerSprite.Drawable::X());*/
      xDirection = false;
      yDirection = false;
      //bullets.setPlayerDir(true);
      shoot = true;
      velY = 0;
      velX = 0;
      playerSprite.setStop(true);
    }
    if(playDir == 1) {
      //std::cout << "In player move right" <<std::endl;
      playerSprite.setRight(true);
      playerSprite.setLeft(false);
      playerSprite.setUp(false);
      //playerSprite.setDown(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(true);
      reverse = false;
      shoot = true;
      xDirection = true;
      if (playerSprite.Drawable::X() < (Gamedata::getInstance().getXmlInt("world/width") - Gamedata::getInstance().getXmlInt("player/width"))) {
        velX = 350;
      }
    }
    if(playDir == 2) {
      //std::cout << "In player move left" << std::endl;
      playerSprite.setLeft(true);
      playerSprite.setRight(false);
      //playerSprite.setUp(false);
      //playerSprite.setDown(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(false);
      shoot = true;
      xDirection = true;
      reverse = true;
      if (playerSprite.Drawable::X() > 0) {
        velX = -350;
      }
    }
    if(playDir == 3) {
      //std::cout << "player up" << std::endl;
      playerSprite.setUp(true);
      playerSprite.setDown(false);
      //playerSprite.setRight(false);
      //playerSprite.setLeft(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(true);
      shoot = false;
      yDirection = true;
      //std::cout << "Y():" << playerSprite.Drawable::Y() << std::endl;
      //std::cout << "worldheignt:" << playerSprite.getWorldHeight() << std::endl;
      //std::cout << "frameHeight:" << playerSprite.getWorldHeight() << "and difference is:" << playerSprite.getWorldHeight() - playerSprite.getFrameHeight() <<std::endl;
      //if (playerSprite.Drawable::Y() > 0 ) {
      if (playerSprite.Drawable::Y() < (playerSprite.getWorldHeight() - playerSprite.getFrameHeight())) {
         velY = -350;
         //std::cout << "Y():" << playerSprite.Drawable::Y() << std::endl;
         //std::cout << "worldheignt:" << playerSprite.getWorldHeight() << std::endl;
         //std::cout << "frameHeight:" << playerSprite.getWorldHeight() << "and difference is:" << playerSprite.getWorldHeight() - playerSprite.getFrameHeight() <<std::endl;
      }
    }
    if(playDir == 4) {
      //std::cout << "player down" << std::endl;
      playerSprite.setDown(true);
      playerSprite.setUp(false);
      //playerSprite.setRight(false);
      //playerSprite.setLeft(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(true);
      shoot = false;
      yDirection = true;
      //std::cout << "Y():" << playerSprite.Drawable::Y() << std::endl;
      //std::cout << "worldheignt:" << playerSprite.getWorldHeight() << std::endl;
      //std::cout << "frameHeight:" << playerSprite.getWorldHeight() << "and their difference" << playerSprite.getWorldHeight() - playerSprite.getFrameHeight() <<std::endl;
      //if (playerSprite.Drawable::Y() < (playerSprite.getWorldHeight() - playerSprite.getFrameHeight())) {
      if (playerSprite.Drawable::Y() > 0 ) {
          velY = 350;
         // std::cout << "Y():" << playerSprite.Drawable::Y() << std::endl;
          //std::cout << "worldheignt:" << playerSprite.getWorldHeight() << std::endl;
          //std::cout << "frameHeight:" << playerSprite.getWorldHeight() << "and their difference" << playerSprite.getWorldHeight() - playerSprite.getFrameHeight() <<std::endl;
      }
    }
    if(playDir == 5) {
      //std::cout << "player left and up" << std::endl;
      playerSprite.setLeft(true);
      playerSprite.setRight(false);
      playerSprite.setUp(true);
      playerSprite.setDown(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(false);
      shoot = true;
      yDirection = true;
      reverse = true;
      if (playerSprite.Drawable::Y() < (playerSprite.getWorldHeight() - playerSprite.getFrameHeight())) {
         velY = -350;
      }
      xDirection = true;
      playerSprite.setReverse(true);

      if (playerSprite.Drawable::X() > 0) {
        velX = -350;
      }
    }
    if(playDir == 6) {
      //std::cout << "player left and down" << std::endl;
      playerSprite.setLeft(true);
      playerSprite.setRight(false);
      playerSprite.setUp(false);
      playerSprite.setDown(true);
      playerSprite.setStop(false);
      xDirection = true;
      shoot = true;
      reverse = true;
      playerSprite.setReverse(true);
      bullets.setPlayerDir(false);

      if (playerSprite.Drawable::X() > 0) {
        velX = -350;
      }
      yDirection = true;
      if (playerSprite.Drawable::Y() > 0) {
          velY = 350;
      }
    }
    if(playDir == 7) {
      //std::cout << "player right and up" << std::endl;
      playerSprite.setRight(true);
      playerSprite.setLeft(false);
      playerSprite.setUp(true);
      playerSprite.setDown(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(true);
      shoot = true;
      yDirection = true;
      reverse = false;
      playerSprite.setReverse(false);
      if (playerSprite.Drawable::Y() < (playerSprite.getWorldHeight() - playerSprite.getFrameHeight())) {
         velY = -350;
      }
      xDirection = true;
      playerSprite.setReverse(false);
      if (playerSprite.Drawable::X() < (playerSprite.getWorldWidth() - playerSprite.getFrameWidth())) {
         velX = 350;
      }
    }
    if(playDir == 8) {
      //std::cout << "player right and down" << std::endl;
      playerSprite.setRight(true);
      playerSprite.setLeft(false);
      playerSprite.setUp(false);
      playerSprite.setDown(true);
      bullets.setPlayerDir(true);
      playerSprite.setStop(false);
      shoot = true;
      xDirection = true;
      reverse = false;
      playerSprite.setReverse(false);
      if (playerSprite.Drawable::X() < (playerSprite.getWorldWidth() - playerSprite.getFrameWidth())) {
         velX = 350;
      }
      yDirection = true;
      if (playerSprite.Drawable::Y() > 0) {
          velY = 350;
      }
    }
    if(playDir == 9) {
      //std::cout << "player shoot" << std::endl;
      shoot = true;
      Vector2f pos(0, 0);
      float xVelocity = 0;
        //if(playerSprite.velocityX() > 0){
        //if(velX > 0) {
           //std::cout << "incrementing bullet velX" << std::endl;
        if(!reverse) {
           pos[0] = playerSprite.Drawable::X() + incrForwardPosition[0];
           pos[1] = playerSprite.Drawable::Y() + incrForwardPosition[1];
           xVelocity = playerSprite.Drawable::velocityX() + bulletSpeed;
        }
        else{
          //std::cout << "?????????????decrementing bullet velX????????????" << std::endl;
          pos[0] = playerSprite.Drawable::X() + incrReversePosition[0];
          pos[1] = playerSprite.Drawable::Y() + incrReversePosition[1];
          xVelocity = playerSprite.Drawable::velocityX() - bulletSpeed;
        }
        bullets.shoot(pos, Vector2f(xVelocity, 0));
        //std::cout << "flag returned by bullets shoot:" << flag << std::endl;
        playDir = 0;
        //playerSprite.setStop(true);
    }
    if(playDir == 10) {
      //std::cout << "player manager up and left and shoot" << std::endl;
      playerSprite.setLeft(true);
      playerSprite.setRight(false);
      playerSprite.setUp(true);
      playerSprite.setDown(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(false);
      shoot = true;
      yDirection = true;
      reverse = true;
      if (playerSprite.Drawable::Y() < (playerSprite.getWorldHeight() - playerSprite.getFrameHeight())) {
         velY = -350;
      }
      xDirection = true;
      playerSprite.setReverse(true);

      if (playerSprite.Drawable::X() > 0) {
        velX = -350;
      }
      Vector2f pos(0, 0);
      float xVelocity = 0;
        //if(playerSprite.velocityX() > 0){
        //if(velX > 0) {
           //std::cout << "incrementing bullet velX" << std::endl;
        if(!reverse) {
           pos[0] = playerSprite.Drawable::X() + incrForwardPosition[0];
           pos[1] = playerSprite.Drawable::Y() + incrForwardPosition[1];
           xVelocity = playerSprite.Drawable::velocityX() + bulletSpeed;
        }
        else{
          //std::cout << "?????????????decrementing bullet velX????????????" << std::endl;
          pos[0] = playerSprite.Drawable::X() + incrReversePosition[0];
          pos[1] = playerSprite.Drawable::Y() + incrReversePosition[1];
          xVelocity = playerSprite.Drawable::velocityX() - bulletSpeed;
        }
        bullets.shoot(pos, Vector2f(xVelocity, 0));
        //std::cout << "flag returned by bullets shoot:" << flag << std::endl;
        playDir = 0;
    }
    if(playDir == 11) {
      //std::cout << "player manager down and left and shoot" << std::endl;
      playerSprite.setLeft(true);
      playerSprite.setRight(false);
      playerSprite.setUp(false);
      playerSprite.setDown(true);
      playerSprite.setStop(false);
      xDirection = true;
      shoot = true;
      reverse = true;
      playerSprite.setReverse(true);
      bullets.setPlayerDir(false);

      if (playerSprite.Drawable::X() > 0) {
        velX = -350;
      }
      yDirection = true;
      if (playerSprite.Drawable::Y() > 0) {
          velY = 350;
      }
      Vector2f pos(0, 0);
      float xVelocity = 0;
        //if(playerSprite.velocityX() > 0){
        //if(velX > 0) {
           //std::cout << "incrementing bullet velX" << std::endl;
        if(!reverse) {
           pos[0] = playerSprite.Drawable::X() + incrForwardPosition[0];
           pos[1] = playerSprite.Drawable::Y() + incrForwardPosition[1];
           xVelocity = playerSprite.Drawable::velocityX() + bulletSpeed;
        }
        else{
          //std::cout << "?????????????decrementing bullet velX????????????" << std::endl;
          pos[0] = playerSprite.Drawable::X() + incrReversePosition[0];
          pos[1] = playerSprite.Drawable::Y() + incrReversePosition[1];
          xVelocity = playerSprite.Drawable::velocityX() - bulletSpeed;
        }
        bullets.shoot(pos, Vector2f(xVelocity, 0));
        //std::cout << "flag returned by bullets shoot:" << flag << std::endl;
        playDir = 0;
    }
    if(playDir == 12) {
      //std::cout << "player manager up and right and shoot" << std::endl;
      playerSprite.setRight(true);
      playerSprite.setLeft(false);
      playerSprite.setUp(true);
      playerSprite.setDown(false);
      playerSprite.setStop(false);
      bullets.setPlayerDir(true);
      shoot = true;
      yDirection = true;
      reverse = false;
      playerSprite.setReverse(false);
      if (playerSprite.Drawable::Y() < (playerSprite.getWorldHeight() - playerSprite.getFrameHeight())) {
         velY = -350;
      }
      xDirection = true;
      playerSprite.setReverse(false);
      if (playerSprite.Drawable::X() < (playerSprite.getWorldWidth() - playerSprite.getFrameWidth())) {
         velX = 350;
      }
      Vector2f pos(0, 0);
      float xVelocity = 0;
        //if(playerSprite.velocityX() > 0){
        //if(velX > 0) {
           //std::cout << "incrementing bullet velX" << std::endl;
        if(!reverse) {
           pos[0] = playerSprite.Drawable::X() + incrForwardPosition[0];
           pos[1] = playerSprite.Drawable::Y() + incrForwardPosition[1];
           xVelocity = playerSprite.Drawable::velocityX() + bulletSpeed;
        }
        else{
          //std::cout << "?????????????decrementing bullet velX????????????" << std::endl;
          pos[0] = playerSprite.Drawable::X() + incrReversePosition[0];
          pos[1] = playerSprite.Drawable::Y() + incrReversePosition[1];
          xVelocity = playerSprite.Drawable::velocityX() - bulletSpeed;
        }
        bullets.shoot(pos, Vector2f(xVelocity, 0));
        //std::cout << "flag returned by bullets shoot:" << flag << std::endl;
        playDir = 0;
    }
    if(playDir == 13) {
      //std::cout << "player manager down and right and shoot" << std::endl;
      playerSprite.setRight(true);
      playerSprite.setLeft(false);
      playerSprite.setUp(false);
      playerSprite.setDown(true);
      bullets.setPlayerDir(true);
      playerSprite.setStop(false);
      shoot = true;
      xDirection = true;
      reverse = false;
      playerSprite.setReverse(false);
      if (playerSprite.Drawable::X() < (playerSprite.getWorldWidth() - playerSprite.getFrameWidth())) {
         velX = 350;
      }
      yDirection = true;
      if (playerSprite.Drawable::Y() > 0) {
          velY = 350;
      }
      /*Vector2f pos(0, 0);
      float xVelocity = 0;
        //if(playerSprite.velocityX() > 0){
        //if(velX > 0) {
           //std::cout << "incrementing bullet velX" << std::endl;
        if(!reverse) {
           pos[0] = playerSprite.Drawable::X() + incrForwardPosition[0];
           pos[1] = playerSprite.Drawable::Y() + incrForwardPosition[1];
           xVelocity = playerSprite.Drawable::velocityX() + bulletSpeed;
        }
        else{
          //std::cout << "?????????????decrementing bullet velX????????????" << std::endl;
          pos[0] = playerSprite.Drawable::X() + incrReversePosition[0];
          pos[1] = playerSprite.Drawable::Y() + incrReversePosition[1];
          xVelocity = playerSprite.Drawable::velocityX() - bulletSpeed;
        }
        bullets.shoot(pos, Vector2f(xVelocity, 0));*/
        //std::cout << "flag returned by bullets shoot:" << flag << std::endl;
        playDir = 0;
    }
  //std::cout << "shoot flag in player update:" << shoot << std::endl;
  playerSprite.update(ticks);
  playerSprite.Drawable::velocityX(velX);
  playerSprite.Drawable::velocityY(velY);
 
 }
}

bool Player::shooting() {
  Vector2f pos(0, 0);
  float xVelocity = 0;
  //if(playerSprite.velocityX() > 0){
  //if(velX > 0) {
  //std::cout << "incrementing bullet velX" << std::endl;
  if(!reverse) {
    pos[0] = playerSprite.Drawable::X() + incrForwardPosition[0];
    pos[1] = playerSprite.Drawable::Y() + incrForwardPosition[1];
    xVelocity = playerSprite.Drawable::velocityX() + bulletSpeed;
  }
  else{
    //std::cout << "?????????????decrementing bullet velX????????????" << std::endl;
    pos[0] = playerSprite.Drawable::X() + incrReversePosition[0];
    pos[1] = playerSprite.Drawable::Y() + incrReversePosition[1];
    xVelocity = playerSprite.Drawable::velocityX() - bulletSpeed;
  }
  return bullets.shoot(pos, Vector2f(xVelocity, 0));
}

bool Player::collidedWith(const TwowayMultiSprite* obj) {
  if(playerSprite.collidedWith(obj) && !(god)){
    dead = true;
  }
  return(dead);
}

bool Player::bulletHitEnemy(const TwowayMultiSprite* obj) {
  if(bullets.collidedWith(obj)) {
    return true;
  }
  else
    return false;
}

bool Player::checkCollisions(){
  /*if(proj && !projExpl){
    if(Planets::getInstance().checkForCollision(proj->getSprite())){
      explodeShot();
      return true;
    }
  }*/
  return false;
}
