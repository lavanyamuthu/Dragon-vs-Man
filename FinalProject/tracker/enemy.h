#ifndef ENEMY__H
#define ENEMY__H

//#include "multisprite.h"
#include "twowayMultiSprite.h"
#include "player.h"
#include "gamedata.h"

class Enemy {
public:
        //Enemy(MultiSprite *en, float r, float a,bool flag);
        Enemy(TwowayMultiSprite *en, float r, float a,bool flag);
        Enemy(const Enemy& s);
        Enemy& operator=(const Enemy&);

        virtual ~Enemy() { }
        
        void update(Uint32 ticks);
        void draw() const { enemy->draw();  }
        void setActive(bool flag) {
          active = flag;
        }
        bool getActive() {
          return active;
        }

        TwowayMultiSprite *getEnemy() { return(enemy); }

private:
        TwowayMultiSprite *enemy;
        float range;
        float attackSpeed;
        float normalX;
        float normalY;
        bool active;
};

#endif
