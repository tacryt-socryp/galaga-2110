
#include "myLib.h"
#include "handleLogic.h"

void createObject(MOVOBJ* obj, MOVOBJ* oldobj);
void moveObject(MOVOBJ* obj);

Game handleLogic(Game game) {
    Game returnGame = game;

    switch(game.state) {
        case TITLE:
            returnGame = titleLogic(game);
            break;
		case PLAY:
            returnGame = playLogic(game);
            break;
        case GAMEOVER:
            returnGame = gameoverLogic(game);
            break;
    }

    return returnGame;
}

Game titleLogic(Game game) {
    return game;
}

Game playLogic(Game game) {
    MOVOBJ *cur;
    MOVOBJ *old;
    // initialization should only happen once
    for (int i = 0; i < 1; i++) { // where 2 will be replaced by NUMOBJS
        cur = game.objs + i;
        old = game.oldobjs + i;
        
        if (cur->size == NULL) {
            createObject(cur, old);
        } else {
            moveObject(cur);
        }

    }
    return game;
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


Game gameoverLogic(Game game) {
    return game;
}
