#include <cmath>
#include "enemy.h"

/*float distance(float x1, float y1, float x2, float y2) {
  float x = x1 - x2;
  float y = y1 - y2;
  return hypot(x, y);
}*/

Enemy::Enemy(TwowayMultiSprite *en, float r, float a,bool flag):
  enemy(en),
  range(r),
  attackSpeed(a),
  normalX(en->velocityX()),
  normalY(en->velocityY()),
  active(flag)
{ }

void Enemy::update(Uint32 ticks){
  enemy->update(ticks);
}

