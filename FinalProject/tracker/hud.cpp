#include <sstream>
#include <string>
#include <vector>
#include "hud.h"
#include "aaline.h"
#include "ioManager.h"

void Hud::draw(int fps, int seconds) const {
  if(displayHelp) {
    //std::cout << "drawing help hud" << std::endl;
    std::ostringstream convertFps;
    convertFps << fps;
    std::string fpsString = convertFps.str();
    std::ostringstream convertSecond;
    convertSecond << seconds;
    std::string secString = convertSecond.str();

    SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"));
    IOManager *io = &IOManager::getInstance();
    SDL_Surface * const screen = io->getScreen();
    const Uint32 color = SDL_MapRGB(screen->format, red, green, blue);
    Draw_AALine(screen,offsetx,offsety+hudheight/2,offsetx+hudwidth,offsety+hudheight/2,hudheight,0xff, 0xff, 0xff, 0xff/2);
    //Draw_AALine(screen,offset,offset,offset+hudwidth,offset+hudheight,hudheight,0xff, 0xff, 0xff, 0xff/2);
    Draw_AALine(screen,offsetx,offsety,offsetx+hudwidth,offsety,thick,color);
    Draw_AALine(screen,offsetx,offsety,offsetx,hudheight+offsety,thick,color);
    Draw_AALine(screen,offsetx+hudwidth,offsety,offsetx+hudwidth,offsety+hudheight,thick,color);
    Draw_AALine(screen,offsetx,offsety+hudheight,offsetx+hudwidth,offsety+hudheight,thick,color);
    IOManager::getInstance().printMessageAt("Average FPS     : " + fpsString , offsetx+10, offsety+5);
    IOManager::getInstance().printMessageAt("Seconds         : " + secString , offsetx+10, offsety+25);
    IOManager::getInstance().printMessageAt("      Press asdw to control player      ", offsetx+10, offsety+45);
    IOManager::getInstance().printMessageAt("          F1 to view help               "  , offsetx+10, offsety+65); 
    IOManager::getInstance().printMessageAt("       F2 to view bullest count         "  , offsetx+10, offsety+85);
    IOManager::getInstance().printMessageAt("         r to restart game              "  , offsetx+10, offsety+105);
    IOManager::getInstance().printMessageAt("         t to fire bullets              "  , offsetx+10, offsety+125);
    IOManager::getInstance().printMessageAt("     g to play game in god mode         "  , offsetx+10, offsety+145);
    IOManager::getInstance().printMessageAt("This is a game between PLAYER vs DRAGONS"  , offsetx+10, offsety+165);
    IOManager::getInstance().printMessageAt(" There is a dragon egg at end of world  "  , offsetx+10, offsety+185);
    IOManager::getInstance().printMessageAt("which can be seen when player has almost"  , offsetx+10, offsety+205);
    IOManager::getInstance().printMessageAt("reached the end of world. Player has to "  , offsetx+10, offsety+225);
    IOManager::getInstance().printMessageAt("reach the end of world and touch the egg"  , offsetx+10, offsety+245);
    IOManager::getInstance().printMessageAt("  to win the game.The player can bypass "  , offsetx+10, offsety+265);
    IOManager::getInstance().printMessageAt(" the dragons or kill them. Player gets  "  , offsetx+10, offsety+285);
    IOManager::getInstance().printMessageAt(" gets killed by the touch of the dragon "  , offsetx+10, offsety+305);
    IOManager::getInstance().printMessageAt("So dodge or attack the dragon. The green"  , offsetx+10, offsety+325);
    IOManager::getInstance().printMessageAt(" dragon is smart, turns back around and "  , offsetx+10, offsety+345);
    IOManager::getInstance().printMessageAt("   traces when player is close to it    "  , offsetx+10, offsety+365);
  }
}

void Hud::draw(unsigned int bulletlistcnt, unsigned int freelistcnt, int enemies) const {
  //std::cout << "hud draw2" << std::endl;
  if(displayCount) {
    //std::cout << "in draw2" << std::endl;
    std::ostringstream convertbulletList;
    convertbulletList << bulletlistcnt;
    std::string bulletCnt = convertbulletList.str();
    std::ostringstream convertfreeList;
    convertfreeList << freelistcnt;
    std::string freeCnt = convertfreeList.str();
    std::ostringstream convertenemiesCount;
    convertenemiesCount << enemies;
    std::string dragonCnt = convertenemiesCount.str();
    SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"));
    IOManager *io = &IOManager::getInstance();
    SDL_Surface * const screen = io->getScreen();
    const Uint32 color = SDL_MapRGB(screen->format, red, green, blue);
    //offset = 300;
    Draw_AALine(screen,offsetx,offsety+hudheight/2,offsetx+hudwidth,offsety+hudheight/2,hudheight,0xff, 0xff, 0xff, 0xff/2);
    Draw_AALine(screen,offsetx,offsety,offsetx+hudwidth,offsety,thick,color);
    Draw_AALine(screen,offsetx,offsety,offsetx,hudheight+offsety,thick,color);
    Draw_AALine(screen,offsetx+hudwidth,offsety,offsetx+hudwidth,offsety+hudheight,thick,color);
    Draw_AALine(screen,offsetx,offsety+hudheight,offsetx+hudwidth,offsety+hudheight,thick,color);

    //IOManager::getInstance().printMessageAt("FreeList count  : " + freeCnt , offset+10, offset+5);
    //IOManager::getInstance().printMessageAt("Dragons count   : " + dragonCnt , offset+10, offset+25);
    IOManager::getInstance().printMessageAt("FreeList count   : " + freeCnt , offsetx+10, offsety+5);
    IOManager::getInstance().printMessageAt("BulletList count : " + bulletCnt , offsetx+10, offsety+25);
    IOManager::getInstance().printMessageAt("Dragons to face  : " + dragonCnt , offsetx+10, offsety+45);
  }
}
