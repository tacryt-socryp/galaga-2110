
#include "myLib.h"
#include "handleLogic.h"

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
    for (int i = 0; i < 1; i++) { // where 2 will be replaced by NUMOBJS
        cur = game->objs + i;
        old = game->oldobjs + i;
        
        if (cur->size == NULL) {
            createObject(cur, old);
        } else {
            moveObject(cur);
        }

    }

    for (int i = 0; i < 50; i++) { // where 2 will be replaced by NUMOBJS
        cur = game->shots + i;
        
        if (cur->size != NULL) {
            moveShot(cur);
        }

    }
}

void createObject(MOVOBJ* obj, MOVOBJ* oldobj) {
    obj->row = 70;
    obj->col = 110;
    obj->rvel = 1;
    obj->cvel = 1;
    obj->color = WHITE;
    obj->size = 8;
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


void gameoverLogic(Game* game) {
    if (game->shouldDrawBackground) {
        
    }
}
