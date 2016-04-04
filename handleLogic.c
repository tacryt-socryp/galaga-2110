
#include "myLib.h"
#include "handleLogic.h"
#include "qran_tonk.h"

void createObject(MOVOBJ* obj, MOVOBJ* oldobj);
void moveObject(MOVOBJ* obj);
void moveShot(MOVOBJ* obj);
void shootingEnemyLogic(Game* game, MOVOBJ* obj);
void rammingEnemyLogic(MOVOBJ* obj);
void enemyCollisionShip(Game* game, MOVOBJ *ship, MOVOBJ *enemy);

void handleLogic(Game* game) {

    switch(game->state) {
        case TITLE:
            titleLogic(game);
            break;
		case PLAY:
            playLogic(game);
            break;
        case GAMEOVER:
            gameoverLogic(game);
            break;
    }
 
}

void titleLogic(Game* game) {
    if (game->shouldDrawBackground) {
    }
}

void playLogic(Game* game) {
    MOVOBJ *cur;
    MOVOBJ *old;

    // initialization should only happen once
    for (int i = 0; i < game->enemyCount; i++) {
        cur = game->objs + i;
        old = game->oldobjs + i;

        if (cur->size == NULL) {
            createObject(cur, old);
            if (i % 2 == 0) {
                cur->type = SHOOTENEMY;
            } else {
                cur->type = RAMENEMY;
            }
        } else {
            switch (cur->type) {
                case SHIP:
                    // this won't happen
                    break;
                case SHOOTENEMY:
                    shootingEnemyLogic(game, cur);
                    break;
                case RAMENEMY:
                    enemyCollisionShip(game, &game->ship, cur);
                    rammingEnemyLogic(cur);
                    break;
            }
        }

    }

    MOVOBJ *obj;
    for (int i = 0; i < 50; i++) {
        cur = game->shots + i;
        
        if (cur->size != NULL) {
            moveShot(cur);

            for (int e = 0; e < game->enemyCount; e++) {
                obj = game->objs + e;
                shotCollisionEnemy(obj, cur);
            }

            shotCollisionShip(game, &game->ship, cur);
        }

    }
}

void shootingEnemyLogic(Game* game, MOVOBJ* obj) {
    int shouldNotShoot = (qran() * (200 - 0) >> 15) + 0;
    int DOWN = 0;
    if (shouldNotShoot == 0) {
        createShot(game, obj->row, obj->col, DOWN);
    }

    int shouldChangeDirection = (qran() * (200 - 0) >> 15) + 0;
    if (shouldChangeDirection == 0) {
        obj->cvel = (qran() * (1 + 1) >> 15) - 1;
    }
    moveObject(obj);
}

void rammingEnemyLogic(MOVOBJ* obj) {

    if (obj->isActive == 0) {
        if (obj->row > 40) {
            // possibility to activate ram if not active now
            int shouldNotRam = (qran() * (200 - 0) >> 15) + 0;
            obj->isActive = shouldNotRam == 0;
        }

        if (obj->row > 50) {
            // normal state
            obj->rvel = 0;
            int shouldChangeDirection = (qran() * (200 - 0) >> 15) + 0;
            if (shouldChangeDirection == 0) {
                obj->cvel = (qran() * (1 + 1) >> 15) - 1;
            }
        }
    } else {
        if (obj->rvel != 3) {
            // begin ram action
            obj->rvel = 3;
            obj->cvel = (qran() * (2 + 2) >> 15) - 2;
        } else if (obj->row < 20) {
            // deactivate ram action when object returns
            obj->rvel = 2;
            obj->isActive = 0;
        }
    }

    moveObject(obj);
}

void createObject(MOVOBJ* obj, MOVOBJ* oldobj) {
    obj->row = 60;
    obj->col = 115;
    obj->rvel = 0;
    obj->cvel = 1;
    obj->color = WHITE;
    obj->size = 5;
    oldobj = obj;
}

void moveObject(MOVOBJ* obj) {
    obj->row += obj->rvel;
    obj->col += obj->cvel;

    if (obj->row<0) {
        obj->row = 0;
        obj->rvel = - obj->rvel;
    }

    if (obj->row > 159-obj->size+1) {
        obj->row = 0;
        // obj->row = 159-obj->size+1;
        // obj->rvel = -obj->rvel;
    }
        
    if (obj->col < 0) {
        obj->col = 0;
        obj->cvel = -obj->cvel;
    }

    if(obj->col > 239-obj->size+1) {
        obj->col= 239-obj->size+1;
        obj->cvel = -obj->cvel;
    }
}

void moveShot(MOVOBJ* shot) {
    shot->row += shot->rvel;
    shot->col += shot->cvel;

    if ((shot->row < 0) || (shot->row > 159 - shot->size + 1) ||
            (shot->col < 0) || (shot->col > 239 - shot->size + 1)) {
        shot->rvel = 0;
        shot->cvel = 0;
        shot->row = 0;
        shot->size = 0;
    }
}

void shotCollisionEnemy(MOVOBJ *obj, MOVOBJ *shot) {
    int collide = collision(obj, shot);

    if (collide) {
        obj->size = 0;
        obj->col=0;
        obj->row=0;
    }

}

void shotCollisionShip(Game* game, MOVOBJ *obj, MOVOBJ *shot) {
    int collide = collision(obj, shot);
    if (collide) {
        game->lives--;
        if (game->lives <= 0) {
            game->state = GAMEOVER;
            game->shouldDrawBackground = 1;
        }
    }

}

void enemyCollisionShip(Game* game, MOVOBJ *ship, MOVOBJ *enemy) {
    int collide = collision(ship, enemy);
    if (collide) {
        game->lives--;
        enemy->size = 0;
        enemy->col=0;
        enemy->row=0;
        if (game->lives <= 0) {
            game->state = GAMEOVER;
            game->shouldDrawBackground = 1;
        }
    }

}

void gameoverLogic(Game* game) {
    if (game->shouldDrawBackground) {
    }
}
