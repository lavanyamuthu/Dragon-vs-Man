#ifndef HUD__H
#define HUD__H

#include<string>
#include"gamedata.h"
//#include "health.h"

class Hud {
public:
  Hud(const bool vis = true,const bool vis1 = false) : 
      displayHelp(vis),
      displayCount(vis1),
      height(Gamedata::getInstance().getXmlFloat("view/height")),
      width(Gamedata::getInstance().getXmlFloat("view/width")),
      hudheight(Gamedata::getInstance().getXmlFloat("hud/height")),
      hudwidth(Gamedata::getInstance().getXmlFloat("hud/width")),
      offsetx(Gamedata::getInstance().getXmlFloat("hud/offsetx")),
      offsety(Gamedata::getInstance().getXmlFloat("hud/offsety")),
      thick(Gamedata::getInstance().getXmlFloat("hud/thick")), 
      red(Gamedata::getInstance().getXmlFloat("hud/red")),
      green(Gamedata::getInstance().getXmlFloat("hud/green")),
      blue(Gamedata::getInstance().getXmlFloat("hud/blue")) {}

  virtual ~Hud() {}

  void sethelpDetails(bool flag) { displayHelp = flag; }
  bool gethelpDetails() const { return displayHelp; }
  void setcountDetails(bool flag1) { displayCount = flag1; }
  bool getcountDetails() const { return displayCount; }
  void setOffsetx(float a) { offsetx = a; }
  void setOffsety(float b) { offsety = b; }
  void sethudHeight(float c) { hudheight = c; }
  void sethudWidth(float d) { hudwidth = d; }
  void togglehelpDetails() {
    if(displayHelp)
       displayHelp = false;
    else
       displayHelp = true;
    //std::cout << "displayHelp:" << displayHelp << std::endl;
  }
  void togglecountDetails() {
    if(displayCount)
       displayCount = false;
    else
       displayCount = true;
    //std::cout << "displayCount:" << displayCount << std::endl;
  }
  //bool getToggle() const { return display; }
  //void setToggle(const bool rhs) { display = rhs; }
  void draw(int fps,int seconds) const;
  void draw(unsigned int bulletlistcnt, unsigned int freelistcnt, int enemies) const;
  //void update(unsigned ticks);

private:
  bool displayHelp;
  bool displayCount;
  float height;
  float width;
  float hudheight;
  float hudwidth;
  float offsetx;
  float offsety;
  float thick;
  int red;
  int green;
  int blue;
  //bool helpDetails;
  //bool countDetails;
  Hud& operator=(const Hud rhs);
};

#endif

