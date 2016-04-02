
#include "myLib.h"
#include "handleLogic.h"
#include "qran_tonk.h"

void createObject(MOVOBJ* obj, MOVOBJ* oldobj);
void moveObject(MOVOBJ* obj);
void moveShot(MOVOBJ* obj);

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
        } else {
            moveObject(cur);
            int shouldShoot = (qran() * (100 - 0) >> 15) + 0;
            int DOWN = 0;
            if (shouldShoot == 0) {
                createShot(game, cur->row, cur->col, DOWN);
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
                shotCollisionEnemy(game, obj, cur);
            }

            shotCollisionShip(game, &game->ship, cur);
        }

    }
}

void createObject(MOVOBJ* obj, MOVOBJ* oldobj) {
    obj->row = 70;
    obj->col = 110;
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
        obj->row = 159-obj->size+1;
        obj->rvel = -obj->rvel;
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

void shotCollisionEnemy(Game* game, MOVOBJ *obj, MOVOBJ *shot) {
    int collide = collision(obj, shot);

    if (collide) {
        obj->size = 0;
        obj->col=0;
        obj->row=0;
        game->enemyCount--;
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


void gameoverLogic(Game* game) {
    if (game->shouldDrawBackground) {
        *game = game_new(GAMEOVER);
        game->shouldDrawBackground = 0;
    }
}
