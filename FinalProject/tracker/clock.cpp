#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  totalTicks(0),
  started(false), 
  paused(false), 
  sloMo(false), 
  sumOfTicks(SDL_GetTicks()),
  starttime(0),
  startpausetime(0),
  stoppausetime(0),
  starttoggle(0),
  stoptoggle(0),
  prevticks(0),
  currticks(0),
  averageFps(0),
  frameCapRate(Gamedata::getInstance().getXmlInt("frameCap")), 
  fpsque(100,0),
  pos( Gamedata::getInstance().getXmlInt("clock/locX"),
       Gamedata::getInstance().getXmlInt("clock/locY"))
  {
  start();
}

void Clock::draw() const { 
  IOManager::getInstance().
    printMessageValueAt("Seconds   : ", getSeconds(), pos[0], pos[1]);
  IOManager::getInstance().
    printMessageValueAt("Averagefps: ", getFps(), pos[0], pos[1]+20);
}

unsigned int Clock::getGameTicks() {
 if(sloMo && !paused)
   return starttoggle;
 if(paused)
   return startpausetime;
 return SDL_GetTicks() - starttime;
}

unsigned int Clock::Delay() const {
  if(averageFps <= frameCapRate) return 0;
  unsigned int delay = std::max(0.0,1000.0/frameCapRate+0.5 - ticks);
  SDL_Delay(delay);
  return delay;
}

void Clock::update() { 
  totalTicks = SDL_GetTicks();
  if(paused && started)
    ticks = 0;
  else if(sloMo && !paused)
    ticks = 1;
  else {
    totalTicks = getGameTicks();
    ticks = totalTicks - prevticks;
    unsigned int delay = Delay();
    prevticks = totalTicks+delay;
    ticks += delay;
  }
  if(!paused)
  {
    if(fpsque.size()==1000)
     {
        fpsque.pop_front();
        fpsque.push_back(1000/ticks);
     }
     else
     {
       fpsque.push_back(1000/ticks);
     }
     for(unsigned int i=0;i<fpsque.size();i++)
     {
       averageFps+=fpsque[i];
     }
     averageFps = averageFps/fpsque.size();
  }
  sumOfTicks += ticks;
}

unsigned int Clock::getTicksSinceLastFrame() const {
  return ticks;
}

void Clock::toggleSloMo() {
  std::cout << "toggle implementation" << std::endl;
  if(!sloMo && started) {
    starttoggle = SDL_GetTicks() - starttime;
    starttime = starttoggle;
    sloMo = true;
  }
}

void Clock::untoggleSloMo() {
  std::cout << "untoggle implementation" << std::endl;
  if(sloMo && started) {
    stoptoggle = SDL_GetTicks() - starttoggle;
    starttime = stoptoggle;
    sloMo = false;
  }
  if(ticks > 0)
    ticks = 1/ticks;
  else
    ticks = 0;
}

int Clock::getFps() const { 
  if ( ticks > 0 ) return averageFps;
  return 0;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  starttime=startpausetime=starttoggle=SDL_GetTicks();
}

void Clock::pause() { 
  std::cout << "pause implementation" << std::endl;
  if(started && !paused) {
    startpausetime = SDL_GetTicks() - starttime;
    starttime = startpausetime;
    paused = true;
  }
}

void Clock::unpause() { 
  std::cout << "unpause implementation" << std::endl;
  if(started & paused) {
    stoppausetime = SDL_GetTicks() - startpausetime;
    starttime = stoppausetime;
    paused = false;
  }
}

