#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "vector2f.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"
//#include "sound.h"

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() { 
  //std::cout << "manager destructor" << std::endl;
  //delete eggFrame;
  SDL_FreeSurface(screen);
  SDL_FreeSurface(orbSurface);
  std::vector<ScaledSprite*>::iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    //SDL_FreeSurface((*ptr)->getSurface());
    //delete (*ptr)->getFrame();
    delete (*ptr);
    ++ptr;
  }
  std::vector<Enemy*>::iterator ptr1 = dragons.begin();
  while ( ptr1 != dragons.end() ) {
    //delete (*ptr1)->getEnemy();
    //(*ptr1)->getEnemy()->deleteStrategy();
    //delete (*ptr1)->getEnemy()->getFrame();
    //delete (*ptr1)->getEnemy();
    delete (*ptr1);
    ++ptr1;
  }
  std::list<ExplodingSprite*>::iterator ptr2 = explodingSprites.begin();
  while ( ptr2 != explodingSprites.end() ) {
    delete (*ptr2);
    ++ptr2;
  }

  //delete rockFrame;
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world1("layer1", Gamedata::getInstance().getXmlInt("layer1/factor") ),
  world2("layer2", Gamedata::getInstance().getXmlInt("layer2/factor") ),
  world3("layer3", Gamedata::getInstance().getXmlInt("layer3/factor") ),
  world4("layer5", Gamedata::getInstance().getXmlInt("layer5/factor") ),
  //world5("layer3", Gamedata::getInstance().getXmlInt("layer5/factor") ),
  viewport( Viewport::getInstance() ),
  orbSurface(io.loadAndSet( Gamedata::getInstance().getXmlStr("fireball/file"),
              Gamedata::getInstance().getXmlBool("fireball/transparency")) ),
  //rockFrame( FrameFactory::getInstance().getFrame("rock")  ),
  //eggFrame( FrameFactory::getInstance().getFrame("egg")  ),
  dragons(),
  //sprites(),
  explodingSprites(),
  currentSprite(),
  sprites(),
  egg("egg",Vector2f(1800,200), Vector2f(0,0)),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  enemyCount( Gamedata::getInstance().getXmlInt("numberOfDragons")),
  enemyFaceCount( Gamedata::getInstance().getXmlInt("numberOfDragons") ),
  win(false),
  entry(true),
  eggdrawn(false),
  player("player"),
  //enemy(),
  //hud()
  hudHelp(),
  hudCount(),
  sound()
  //playerHealth()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

  //dragons.push_back(new MultiSprite("dragon"));
  //dragons.push_back(new MultiSprite("greendragon"));
  //int pw = Gamedata::getInstance().getXmlInt("player/width"); 
  //int ph = Gamedata::getInstance().getXmlInt("player/height");
  int ww = Gamedata::getInstance().getXmlInt("world/width");
  int wh = Gamedata::getInstance().getXmlInt("world/height");
  for(int i = 0; i < enemyCount/2; i++) {
    /*dragons.push_back(new Enemy(new MultiSprite("dragon"),
                                &player,
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed")));*/
    /*dragons.push_back(new Enemy(new MultiSprite("dragon",getRandFloat(pw + 50,ww),
                                getRandFloat((ph + 50),
                                wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedX")),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedY"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedY"))),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));*/
    /*dragons.push_back(new Enemy(new TwowayMultiSprite("dragon",getRandFloat(pw + 50,ww),
                                getRandFloat((ph + 50),
                                wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedX")),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedY"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedY"))),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));
    dragons.push_back(new Enemy(new TwowayMultiSprite("greendragon",getRandFloat(pw + 50,ww),
                                getRandFloat((ph + 50),
                                wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedX")),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedY"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedY"))),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));*/
    dragons.push_back(new Enemy(new TwowayMultiSprite("dragon",getRandFloat(ww, 0),
                                getRandFloat(wh, 0),
                                //wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                -Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                Gamedata::getInstance().getXmlInt("dragon/speedY")),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));
    dragons.push_back(new Enemy(new TwowayMultiSprite("greendragon",getRandFloat(0, ww),
                                getRandFloat(0, wh),
                                //wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                -Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                Gamedata::getInstance().getXmlInt("dragon/speedY")),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));
  }
  makeOrbs();
  //makeSprites();
  const Drawable *trackObject = player.getDrawable();
  //currentSprite = player.getDrawable();
  viewport.setObjectToTrack(trackObject);
  //viewport.setObjectToTrack((const Drawable *)player.getDrawable());
}

void Manager::makeOrbs() {
  unsigned numberOfOrbs = Gamedata::getInstance().getXmlInt("numberOfOrbs");
  sprites.reserve( numberOfOrbs );
  for (unsigned i = 0; i < numberOfOrbs; ++i) {
    sprites.push_back( new ScaledSprite("star", orbSurface) );
  }
  sort(sprites.begin(), sprites.end(), ScaledSpriteCompare());
}

void Manager::printOrbs() const {
  for (unsigned i = 0; i < sprites.size(); ++i) {
    std::cout << sprites[i]->getScale() << std::endl;
  }
}

void Manager::draw() const {
  //std::cout << "in manager draw" << std::endl;
  world1.draw();
  for(unsigned int i = 0; i < sprites.size()/4; i++){
    sprites[i]->draw();
  }
  world2.draw();
  for(unsigned int i = sprites.size()/4; i < (sprites.size()*2)/4; i++){
    sprites[i]->draw();
  }
  world3.draw();
  for(unsigned int i = (sprites.size()*2)/4; i < (sprites.size()*3)/4; i++){
    sprites[i]->draw();
  }
  world4.draw();
  for(unsigned int i = (sprites.size()*3)/4; i < sprites.size(); i++){
    sprites[i]->draw();
  }
  /*world5.draw();
  for(unsigned int i = (sprites.size()*4)/5; i < sprites.size(); i++){
    sprites[i]->draw();
  }*/
  //std::cout << "in manager draw1" << std::endl;
  //clock.draw();
  //std::cout << "in manager draw2" << std::endl;
  //std::cout << "player x pos is:" << player.getX() << std::endl;
  //std::cout << "player y pos is:" << player.getY() << std::endl;
  if( (player.getX() > 1750 && (!eggdrawn)) || eggdrawn ) {
    egg.draw();
  }
  player.draw();
  std::vector<Enemy*>::const_iterator ptr1 = dragons.begin();
  while ( ptr1 != dragons.end() ) {
    std::string enemyname = "greendragon";
    if( (*ptr1)->getEnemy()->getName().compare(enemyname) == 0 ) {
       //std::cout << "greendragon pos x:" << (*ptr1)->getEnemy()->X() << "pos y:" << (*ptr1)->getEnemy()->Y() << std::endl;
       //std::cout << "player pos x     :" << player.getDrawable()->X() << "pos y:" << player.getDrawable()->Y()  << std::endl;
       if ( (player.getDrawable()->X() > (*ptr1)->getEnemy()->X()) && (player.getDrawable()->X() < (*ptr1)->getEnemy()->X()+20) &&
            (player.getDrawable()->Y() > (*ptr1)->getEnemy()->Y()) && (player.getDrawable()->Y() < (*ptr1)->getEnemy()->Y()+20)) {
         //std::cout << "greendragon rev chk in manager" << std::endl;
         (*ptr1)->getEnemy()->setSmartenemyrev(true);
      }
    }
    (*ptr1)->draw();
    ++ptr1;
  }

  std::list<ExplodingSprite*>::const_iterator ptr2 = explodingSprites.begin();
  while(ptr2 != explodingSprites.end()) {
    (*ptr2)->draw();
    if( player.getreassemble() ) {
      player.draw();
    }
    ++ptr2;
  }

  if( player.getDead() && !(player.getGod()) ) {
    IOManager::getInstance().printMessageCenteredAt("Game Over... YOU LOSE!!!!!",
    Gamedata::getInstance().getXmlInt("view/height") / 2);
  }
  /*else if(dragons.size() == 0 && !(player.getGod())) {
    //IOManager::getInstance().printMessageCenteredAt("YOU HAVE ALMOST WON THE GAME, GO AND PICK THE DRAGON EGG!!!!",
    //Gamedata::getInstance().getXmlFloat("view/height") / 2);
    //float a = player.getX();
    //float b = player.getY();
    //egg.X(a);
    //egg.Y(b);
    //egg.draw();
    std::cout<< "player.getX()" << player.getX() << "player.getY()" << player.getY() << std::endl;
    if((player.getX() < 1750 && player.getX() > 1800 ) && ( player.getY() < 180 && player.getY() > 220))
      IOManager::getInstance().printMessageCenteredAt("YOU HAVE WON THE GAME!!!!",Gamedata::getInstance().getXmlFloat("view/height") / 2);
  }*/
  int activeCount = 0;
  std::vector<Enemy*>::const_iterator ptr3 = dragons.begin();
  while ( ptr3 != dragons.end() ) {
    if ((*ptr3)->getActive())
       activeCount++;
    ++ptr3;
  }

  //std::cout << "in manager draw7" << std::endl;
  if(hudHelp.gethelpDetails()==true)
     hudHelp.draw(clock.getFps(),clock.getSeconds());
  if(hudCount.getcountDetails()==true)
    hudCount.draw(player.getBulletListCount(), player.getFreeListCount(),activeCount);
  if( !(player.getGod()) && !(player.getDead()) ) {
    //std::cout << "manager draw, X():" << player.getX() << std::endl;
    //if (player.getX() > 1942 ) {
     if(activeCount == 0) {
      //std::cout<< "player.getX()" << player.getX() << "player.getY()" << player.getY() << std::endl;
      //if((player.getX() <= 1880 && player.getX() >= 1770 ))
      //   std::cout << "player satisfies x val for winning:" << player.getX() << std::endl;
      //if(( player.getY() <= 220 && player.getY() >= 180))
      //  std::cout << "player statistics y val for winning:" << player.getY() << std::endl;
      if((player.getX() <= 1870 && player.getX() >= 1770 ) && ( player.getY() <= 270 && player.getY() >= 170)) {
        IOManager::getInstance().printMessageCenteredAt("YOU HAVE WON THE GAME!!!!",Gamedata::getInstance().getXmlFloat("view/height") / 2);
      }
      //IOManager::getInstance().printMessageCenteredAt("YOU HAVE WON THE GAME!!!!",Gamedata::getInstance().getXmlFloat("view/height") / 2);
    }
  }
  viewport.draw();

  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  //std::cout << "In manager update" << std::endl;
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  //player.update(ticks);
  //enemy.update(ticks);
  int ActiveCount = 0;
  std::vector<Enemy*>::iterator ptr3 = dragons.begin();
  while ( ptr3 != dragons.end() ) {
    if ((*ptr3)->getActive())
       ActiveCount++;
    ++ptr3;
  }
if (!(win)) {
  if( !(player.getGod()) && !(player.getDead()) ) {
    //std::cout << "manager draw, X():" << player.getX() << std::endl;
    //if (player.getX() > 1942 ) {
     if(ActiveCount == 0) {
      //std::cout<< "player.getX()" << player.getX() << "player.getY()" << player.getY() << std::endl;
      if((player.getX() <= 1870 && player.getX() >= 1770 ) && ( player.getY() <= 270 && player.getY() >= 170)) {
        win =true;
      }
      //IOManager::getInstance().printMessageCenteredAt("YOU HAVE WON THE GAME!!!!",Gamedata::getInstance().getXmlFloat("view/height") / 2);
    }
  }
  player.setreassemble(false);
  if(player.getX() > 1800)
    eggdrawn = true;
  std::list<ExplodingSprite*>::iterator ptr2 = explodingSprites.begin();
  while ( ptr2 != explodingSprites.end() ) {
    (*ptr2)->update(ticks);
    std::string playername = "player";
    //std::cout << "exploding sprite name is:" << (*ptr2)->getNameofExplodingSprite() << std::endl;
    if( ((*ptr2)->getNameofExplodingSprite()).compare(playername) == 0) {
      //std::cout << "It is the player's chunk, should reassemble it" << std::endl;
      if ( (*ptr2)->chunkCount() == 0 ) {
        //delete (*ptr2);
        //*ptr2 = NULL;
        //ptr2 = explodingSprites.erase(ptr2);
        player.setreassemble(true);
        //++ptr2;
      }  
    }
    ++ptr2;
  }
  //std::cout << "In manager update1" << std::endl;

  std::vector<Enemy*>::iterator ptr1 = dragons.begin();
  while ( ptr1 != dragons.end() ) {
    (*ptr1)->update(ticks);
    if( ((*ptr1)->getEnemy()->X()) < (player.getDrawable()->X()-10) ) {
      //std::cout << "dragon passed player" << std::endl;
      (*ptr1)->setActive(false);
    }
    if( player.bulletHitEnemy((*ptr1)->getEnemy()) ) {
      //std::cout << "enemy explosion" << std::endl;
      sound[1];
      explodingSprites.push_back(new ExplodingSprite((*ptr1)->getEnemy()->getName(),(*ptr1)->getEnemy()->getPosition(),(*ptr1)->getEnemy()->getVelocity(),(*ptr1)->getEnemy()->getFrame()));
      //delete (*ptr1)->getEnemy()->getFrame();
      //delete (*ptr1)->getEnemy();
      delete *ptr1;
      ptr1 = dragons.erase(ptr1);
      --enemyFaceCount;
    }
    else if ( !player.getDead() && player.collidedWith((*ptr1)->getEnemy()) && !(player.getGod()) ) {
      //std::cout << "collision happend" << std::endl;
      //io.printMessageAt("*** Oops ***, collision!",20,20);
      //int n = 1;
      sound[1];
      explodingSprites.push_back(new ExplodingSprite(player.getDrawable()->getName(),player.getDrawable()->getPosition(),player.getDrawable()->getVelocity(),player.getDrawable()->getFrame()));
      //std::cout << "collision happend" << std::endl;
      ++ptr1;
    }
    else
      ++ptr1;
  }
  //std::cout << "In manager update2" << std::endl;

  std::vector<ScaledSprite*>::iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  //std::cout << "In manager update3" << std::endl;

  player.update(ticks);
  //std::cout << "In manager update4" << std::endl;
  world1.update();
  //std::cout << "In manager update5" << std::endl;
  world2.update();
  //std::cout << "In manager update6" << std::endl;
  world3.update();
  world4.update();
  //world5.update();
  //std::cout << "In manager update7" << std::endl;
  viewport.update(); // always update viewport last
  //std::cout << "manager update end" << std::endl;
  }
}

void Manager::reset() {
  //std::cout << "manger reset" << "y pos in game.xml is:"<< Gamedata::getInstance().getXmlInt("player/srcY") <<std::endl;
  enemyCount = Gamedata::getInstance().getXmlInt("numberOfDragons");
  //std::cout << "manger reset" << enemyCount <<std::endl;
  //player("player");
  player.setvelX(Gamedata::getInstance().getXmlInt("player/speedX"));
  player.setvelY(Gamedata::getInstance().getXmlInt("player/speedY"));
  //std::cout << "in begin1 player x pos is:" << player.getX() << std::endl;
  //std::cout << "in begin1 player y pos is:" << player.getY() << std::endl;
  //std::cout << "srcY in between:" <<Gamedata::getInstance().getXmlInt("player/srcY") << std::endl;
  player.setX(Gamedata::getInstance().getXmlInt("player/srcX"));
  player.setY(Gamedata::getInstance().getXmlInt("player/srcY"));
  //std::cout << "in begin2 player x pos is:" << player.getX() << std::endl;
  //std::cout << "in begin2 player y pos is:" << player.getY() << std::endl;
  player.setDead(false);
  //bool flag = false;
  //player.getPlayerSprite()->setLeft(flag);
  //player.getPlayerSprite()->setRight(false);
  //player.getPlayerSprite()->setUp(false);
  //player.getPlayerSprite()->setDown(false);
  hudHelp.sethelpDetails(false);
  hudCount.setcountDetails(false);
  //std::cout << "manager reset1" << std::endl;
  //int pw = Gamedata::getInstance().getXmlInt("player/width");
  //int ph = Gamedata::getInstance().getXmlInt("player/height");
  int ww = Gamedata::getInstance().getXmlInt("world/width");
  int wh = Gamedata::getInstance().getXmlInt("world/height");
  
  std::vector<Enemy*>::iterator ptr1 = dragons.begin();
  while ( ptr1 != dragons.end() ) {
    //(*ptr1)->getEnemy()->deleteStrategy();
    //delete (*ptr1)->getEnemy()->getFrame();
    //delete (*ptr1)->getEnemy();
    delete (*ptr1);
    ptr1 = dragons.erase(ptr1);
    //++ptr1;
  }
  //std::cout << "manager reset2" << std::endl;
  std::list<ExplodingSprite*>::iterator ptr2 = explodingSprites.begin();
  while ( ptr2 != explodingSprites.end() ) {
    delete (*ptr2);
    ptr2 = explodingSprites.erase(ptr2);
    //++ptr2;
  }
  //std::cout << "manager reset3" << std::endl;
  for(int i = 0; i < enemyCount/2; i++) {
    /*dragons.push_back(new Enemy(new MultiSprite("dragon"),
                                &player,
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed")));*/
    /*dragons.push_back(new Enemy(new TwowayMultiSprite("dragon",getRandFloat(pw + 50,ww),
                                getRandFloat((ph + 50),
                                wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedX")),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedY"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedY"))),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));
    dragons.push_back(new Enemy(new TwowayMultiSprite("greendragon",getRandFloat(pw + 50,ww),
                                getRandFloat((ph + 50),
                                wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedX")),
                                (rand()%2?1:-1)*getRandFloat(-Gamedata::getInstance().getXmlInt("dragon/speedY"),
                                        Gamedata::getInstance().getXmlInt("dragon/speedY"))),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));*/
    dragons.push_back(new Enemy(new TwowayMultiSprite("dragon",getRandFloat(ww, 0),
                                getRandFloat(wh, 0),
                                //wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                -Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                Gamedata::getInstance().getXmlInt("dragon/speedY")),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));
    dragons.push_back(new Enemy(new TwowayMultiSprite("greendragon",getRandFloat(0, ww),
                                getRandFloat(0, wh),
                                //wh - (Gamedata::getInstance().getXmlInt("dragon/height"))),
                                -Gamedata::getInstance().getXmlInt("dragon/speedX"),
                                Gamedata::getInstance().getXmlInt("dragon/speedY")),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackRange"),
                                Gamedata::getInstance().getXmlFloat("dragon/enemyAttackSpeed"),true));
  }
  win = false;
  eggdrawn=false;
  const Drawable *trackObject = player.getDrawable();
  viewport.setObjectToTrack(trackObject);
  player.setX(Gamedata::getInstance().getXmlInt("player/srcX"));
  player.setY(Gamedata::getInstance().getXmlInt("player/srcY"));
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyLeft = false;
  bool keyRight = false;
  bool keyShoot = false;
  bool keyUp = false;
  bool keyDown = false;
  bool F1key = false;
  //bool entry = true;
  clock.start();

  while ( not done ) {
   //if(!entry) {
    //std::cout << "entry over" << std::endl;
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYUP) {
        //std::cout << "SDL key UP" << std::endl;
        if( (keystate[SDLK_w] || keystate[SDLK_UP]) && (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() ) {
          //std::cout << "manager up nad left" << std::endl;
          player.setplayDir(5);
          //keyShoot = false;
        }
        else if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() ) {
          //std::cout << "manager down and left" << std::endl;
          player.setplayDir(6);
          //keyShoot = false;
        }
        else if( (keystate[SDLK_w] || keystate[SDLK_UP]) && (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() )  {
          //std::cout << "manager up and right" << std::endl;
          player.setplayDir(7);
          //keyShoot = false;
        }
        else if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() ) {
          //std::cout << "manager down and right" << std::endl;
          player.setplayDir(8);
          //keyShoot = false;
        }
        else if( keystate[SDLK_a] || keystate[SDLK_LEFT] ) {
          //std::cout << "SDL key UP and left" << std::endl;
          player.setplayDir(2);
          keyLeft = true;
          keyRight = false;
          keyShoot = false;
        }
        else if( (keystate[SDLK_w] || keystate[SDLK_UP]) && !player.getDead() ) {
          //std::cout << "SDL key UP and up" << std::endl;
          player.setplayDir(3);
          keyUp = true;
          keyDown = false;
          keyShoot = false;
        }
        else if( (keystate[SDLK_d] || keystate[SDLK_RIGHT] ) && !player.getDead() ) {
          //std::cout << "sdl key up and manager down" << std::endl;
          player.setplayDir(1);
          keyRight = true;
          keyLeft = false;
          keyShoot = false;
        }
        else if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && !player.getDead() ) {
          //std::cout << "SDL key UP and down" << std::endl;
          player.setplayDir(4);
          keyDown = true;
          keyUp = false;
          keyShoot = false;
        }
        else {
          //std::cout << "in SDL key up stop" << std::endl;
          keyUp = false;
          keyDown = false;
          keyRight = false;
          keyLeft = false;
          keyShoot = false;
          if( !player.getDead() )
             player.setplayDir(0);
        }
        //std::cout << "end of SDL key up" << std::endl;
      }
      if(event.type == SDL_KEYDOWN) {
        //std::cout << "SDL key DOWN" << std::endl;
        if ( keystate[SDLK_ESCAPE] || keystate[SDLK_q] ) { 
          //std::cout << "escape" << std::endl;
          done = true;
          break;
        }
        if ( keystate[SDLK_F1] ) {
          //std::cout << "f1" << std::endl;
          if(!hudHelp.gethelpDetails()) {
            //hud1.setToggle(true);
            hudHelp.togglehelpDetails();
            hudHelp.togglecountDetails();
            //hudHelp.sethelpDetails(true);
            //hudHelp.setcountDetails(false);
            F1key = true;
          }
          else {
            //hud1.setToggle(false);
            //hudHelp.sethelpDetails(false);
            //hudHelp.setcountDetails(false);
            hudHelp.togglehelpDetails();
            hudHelp.togglecountDetails();
            F1key = false;
          }
          break;
        }
        if ( keystate[SDLK_F2] ) {
          //std::cout << "f2" << std::endl;
          if(!hudCount.getcountDetails()) {
            //hud2.setToggle(true);
            //std::cout << "offsetx:" << Gamedata::getInstance().getXmlFloat("hud2/offsetx") << std::endl;
            //std::cout << "offsety:" << Gamedata::getInstance().getXmlFloat("hud2/offsety") << std::endl;
            //std::cout << "hudHeight:" << Gamedata::getInstance().getXmlFloat("hud2/height") << std::endl;
            //std::cout << "hudWidth:" << Gamedata::getInstance().getXmlFloat("hud2/width") << std::endl;
            hudCount.sethelpDetails(false);
            hudCount.setcountDetails(true);
            hudCount.setOffsetx(640.0);
            hudCount.setOffsety(20.0);
            hudCount.sethudHeight(65.0);
            hudCount.sethudWidth(190.0);
            //hudCount.setOffsetx(Gamedata::getInstance().getXmlFloat("hud2/offsetx"));
            //hudCount.setOffsety(Gamedata::getInstance().getXmlFloat("hud2/offsetx"));
            //hudCount.sethudHeight(Gamedata::getInstance().getXmlFloat("hud2/height"));
            //hudCount.sethudWidth(Gamedata::getInstance().getXmlFloat("hud2/width"));
            //F1key = true;
          }
          else {
            //hud2.setToggle(false);
            hudCount.sethelpDetails(false);
            hudCount.setcountDetails(false);
            //F1key = false;
          }
          break;
        }
        /*if ( keystate[SDLK_p] && !player.getDead() ) {
          //std::cout << "pause" << std::endl;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }*/
        if ( keystate[SDLK_r] ) {
          //std::cout << "manager reset" << std::endl;
          keyLeft = false;
          keyRight = false;
          keyShoot = false;
          keyUp = false;
          keyDown = false;
          player.setGod(false);
          player.setplayDir(0);
          //F1key = false;
          reset();
          //done = false;
        }
        if ( keystate[SDLK_g] ) {
          //std::cout << "god mode" << std::endl;
          if(!player.getGod()) {
            keyLeft = false;
            keyRight = false;
            keyShoot = false;
            keyUp = false;
            keyDown = false;
            //F1key = false;
            player.setGod(true);
            //done = false;
          }
          else {
            //keyLeft = false;
            //keyRight = false;
            //keyShoot = false;
            //keyUp = false;
            //keyDown = false;
            //F1key = false;
            player.setGod(false);
          }
        }
        if ( keystate[SDLK_F4] && !makeVideo && !player.getDead() ) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if ( keystate[SDLK_t] && !player.getDead() ) {
        //if(event.key.keysym.sym == SDLK_t) {
           //std::cout << "manager shoot" << std::endl;
           if(!keyShoot) {
             sound[0];
             player.setplayDir(9);
             //std::cout << "manager shoot" << std::endl;
             if(player.shooting()) {
               sound[0];
               //std::cout << "player shooting called" << std::endl;
             }
             keyShoot = true;
           }
        }
        if ( (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() ){
           //std::cout << "manager right" << std::endl;
           if(!keyRight && !keyLeft) {
            player.setplayDir(1);
            keyRight = true;
            keyLeft = false;
            keyShoot = false;
          }
          if( !keyRight && keyLeft ) {
            player.setplayDir(0);
          }
        }
        if ( (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() ) {
           //std::cout << "manager left" << std::endl;
           if(!keyLeft && !keyRight) {
            player.setplayDir(2);
            keyLeft = true;
            keyRight = false;
            keyShoot = false;
          }
          if( !keyLeft && keyRight ) {
            player.setplayDir(0);
          }
        }
        if( (keystate[SDLK_w] || keystate[SDLK_UP]) && !player.getDead() ) {
          if(!keyUp && !keyDown) {
            player.setplayDir(3);
            keyUp = true;
            keyDown = false;
            keyShoot = false;
          }
          if(!keyUp && keyDown) {
            player.setplayDir(0);
          }
        }
        if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && !player.getDead() ) {
          //std::cout << "manager down" << std::endl;
          if(!keyDown && !keyUp) {
            player.setplayDir(4);
            keyDown = true;
            keyUp = false;
            keyShoot = false;
          }
          if(!keyDown && keyUp) {
             player.setplayDir(0);
          }
        }
        if( (keystate[SDLK_w] || keystate[SDLK_UP]) && (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() ) {
          //std::cout << "manager up nad left" << std::endl;
          player.setplayDir(5);
          //keyShoot = false;
        }
        if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() ) {
          //std::cout << "manager down and left" << std::endl;
          player.setplayDir(6);
          //keyShoot = false;
        }
        if( (keystate[SDLK_w] || keystate[SDLK_UP]) && (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() )  {
          //std::cout << "manager up and right" << std::endl;
          player.setplayDir(7);
          //keyShoot = false;
        }
        if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() ) {
          //std::cout << "manager down and right" << std::endl;
          player.setplayDir(8);
          //keyShoot = false;
        }
        if( (keystate[SDLK_w] || keystate[SDLK_UP]) && (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() && keystate[SDLK_t] ) {
          //std::cout << "manager up nad left" << std::endl;
          player.setplayDir(10);
          //keyShoot = false;
        }
        if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && (keystate[SDLK_a] || keystate[SDLK_LEFT]) && !player.getDead() && keystate[SDLK_t] ) {
          //std::cout << "manager down and left" << std::endl;
          player.setplayDir(11);
          //keyShoot = false;
        }
        if( (keystate[SDLK_w] || keystate[SDLK_UP]) && (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() && keystate[SDLK_t] )  {
          //std::cout << "manager up and right" << std::endl;
          player.setplayDir(12);
          //keyShoot = false;
        }
        if( (keystate[SDLK_s] || keystate[SDLK_DOWN]) && (keystate[SDLK_d] || keystate[SDLK_RIGHT]) && !player.getDead() && keystate[SDLK_t] ) {
          //std::cout << "manager down and right" << std::endl;
          player.setplayDir(13);
          //keyShoot = false;
        }
      }
    }
    //}
    //std::cout << "before manager draw" << std::endl;
    //if(clock.getSeconds() < 3u && F1key==false) {
    /*if(clock.getSeconds() < 3u) {
      hudHelp.sethelpDetails(true);
      hudHelp.setcountDetails(false);
      //entry = true;
    }
    //if(clock.getSeconds() < 3u && F1key==true) {
    if(clock.getSeconds() < 3u) {
      hudHelp.sethelpDetails(true);
      hudHelp.setcountDetails(false);
      //entry = true;
    }*/
    //if(clock.getSeconds() >= 3u && F1key==false) {
    if( (clock.getSeconds() >= 4u) && hudHelp.gethelpDetails() && F1key==false) {
      hudHelp.togglehelpDetails();
      //std::cout << "in manager sec check" << std::endl;
      //hudHelp.sethelpDetails(false);
      //hudHelp.setcountDetails(false);
      //entry = false;
    }
    //std::cout << "b4 manager draw" << std::endl;
    draw();
    update();
  }
}
