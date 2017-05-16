#include <iostream>
#include "twowayMultiSprite.h"
#include "gamedata.h"
#include "sprite.h"

class Bullet : public TwowayMultiSprite{
public:
  explicit Bullet(const string& name, 
                   const Vector2f& pos, const Vector2f vel, 
                   std::vector<Frame*>& framesL, std::vector<Frame*>& framesR) :
    TwowayMultiSprite(name,pos,vel,framesL,framesR),
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false),
    dir(true),
    position(pos),
    velocity(vel),
    name(name)
  { }
  virtual ~Bullet() {}
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
  //~Bullet() { TwowayMultiSprite::deleteStrategy(); };
  void setDir(bool flag) { dir = flag; }
  bool getDir() const { return dir; }
  //bool collidedWith(const Drawable* obj) const;

private:
  float distance;
  float maxDistance;
  bool tooFar;
  bool dir;
  Vector2f position;
  Vector2f velocity;
 
  const string& name;
};
