
#include "myLib.h"
#include "handleInput.h"

void moveShip(MOVOBJ* obj, int direction);

Game handleInput(Game game) {
    Game returnGame = game;

    switch(game.state) {
        case TITLE:
            returnGame = titleInput(game);
            break;
		case PLAY:
            returnGame = playInput(game);
            break;
        case GAMEOVER:
            returnGame = gameoverInput(game);
            break;
    }

    return returnGame;
}

Game titleInput(Game game) {
    if (KEY_DOWN_NOW(BUTTON_A)) {
        game.state = PLAY;
        game.shouldDrawBackground = 1;
    }
    return game;
}

Game playInput(Game game) {
    if (KEY_DOWN_NOW(BUTTON_LEFT) & ~KEY_DOWN_NOW(BUTTON_RIGHT)) {
        moveShip(&game.ship, -1);
    } else if (KEY_DOWN_NOW(BUTTON_RIGHT) & ~KEY_DOWN_NOW(BUTTON_LEFT)) {
        moveShip(&game.ship, 1);
    }
    return game;
}

void moveShip(MOVOBJ* obj, int direction) {
    // if direction is negative, left, if direction is positive, right
    if (direction < 0) {
        obj->col += -2; 
    } else {
        obj->col += 2;
    }
        
    if (obj->col < 0) {
        obj->col = 0;
        obj->cvel = -obj->cvel;
    }

    if (obj->col > 239 - obj->size + 1) {
        obj->col= 239-obj->size+1;
        obj->cvel = -obj->cvel;
    }
}

Game gameoverInput(Game game) {
    return game;
}
