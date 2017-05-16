#ifndef TWOWAYSPRITE_H
#define TWOWAYSPRITE_H
#include "multisprite.h"
//#include "collisionStrategy.h"

class CollisionStrategy;
class TwowayMultiSprite : public Drawable {
public:
  TwowayMultiSprite(const std::string&);
  TwowayMultiSprite(const TwowayMultiSprite&);
  TwowayMultiSprite(const std::string& n, const Vector2f& pos, const Vector2f& vel,std::vector<Frame*>& fmsLeft,
                    std::vector<Frame*>& fmsRight);
  TwowayMultiSprite(const std::string& name, float x, float y, float sX, float sY);
  TwowayMultiSprite& operator=(const TwowayMultiSprite&); 
  virtual ~TwowayMultiSprite();
  void draw() const;
  void advanceFrame(Uint32 ticks);
  void update(Uint32 ticks);
  const Frame* getFrame() const { return frames[currentFrame]; }
  const std::string getName() const { return spritename; }
  int getWorldWidth() const { return worldWidth; }
  int getWorldHeight() const { return worldHeight; }
  int getFrameWidth() const { return frameWidth; }
  int getFrameHeight() const { return frameHeight; }
  bool collidedWith(const TwowayMultiSprite*) const;
  void deleteStrategy();
  void setReverse(bool flag) {
    reverse = flag;
  }
  bool getReverse() {
    return reverse;
  }
  void setUp(bool flag) {
    up = flag;
  }
  bool getUp() {
    return up;
  }
  void setDown(bool flag) {
    down = flag;
  }
  bool getDown() {
    return down;
  }
  void setLeft(bool flag) {
    left = flag;
  }
  bool getLeft() {
    return left;
  }
  void setRight(bool flag) {
    right = flag;
  }
  bool getRight() {
    return right;
  }
  void setStop(bool flag) {
    stop = flag;
  }
  bool getStop() {
    return stop;
  }
  void setSmartenemyrev(bool flag) {
    smartenemyrev = flag;
  }
  bool getSmartenemyrev() {
    return smartenemyrev;
  }
  void createChunks();
protected:
  //ExplodingSprite* explosion;
  std::vector<Frame *> frames;
  std::vector<Frame *> framesLeft;
  std::vector<Frame *> framesRight;
  std::vector<Frame *> framesUp;
  std::vector<Frame *> framesDown;
  CollisionStrategy *strategy;
  std::string spritename;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  bool reverse;
  bool left;
  bool right;
  bool up;
  bool down;
  bool stop;
  bool smartenemyrev;
  //void advanceFrame(Uint32 ticks);
  //void update(Uint32 ticks);
};
#endif
