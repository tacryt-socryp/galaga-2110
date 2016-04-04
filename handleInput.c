
#include "myLib.h"
#include "handleInput.h"
#include "qran_tonk.h"

void moveShip(MOVOBJ* obj, int direction);
int randSeed = 0;

void handleInput(Game* game) {

    switch(game->state) {
        case TITLE:
            titleInput(game);
            break;
		case PLAY:
            playInput(game);
            break;
        case GAMEOVER:
            gameoverInput(game);
            break;
    }

}

void titleInput(Game* game) {
    randSeed++;
    if (KEY_DOWN_NOW(BUTTON_A)) {
        game->state = PLAY;
        game->shouldDrawBackground = 1;

        sqran(randSeed);
    }
}

void playInput(Game* game) {

    if (KEY_DOWN_NOW(BUTTON_A)) {
        int UP = 1;
        if ((game->shotRateLimiter % game->shipFireRate) == 0) {
            createShot(game, game->ship.row, game->ship.col, UP);
        }

        game->shotRateLimiter++;
    } else {
        game->shotRateLimiter = 0;
    }

    if (KEY_DOWN_NOW(BUTTON_LEFT) & ~KEY_DOWN_NOW(BUTTON_RIGHT)) {
        moveShip(&game->ship, -1);
    } else if (KEY_DOWN_NOW(BUTTON_RIGHT) & ~KEY_DOWN_NOW(BUTTON_LEFT)) {
        moveShip(&game->ship, 1);
    }
}

void moveShip(MOVOBJ* obj, int direction) {
    // if direction is negative, left, if direction is positive, right
    if (direction < 0) {
        obj->col += -3; 
    } else {
        obj->col += 3;
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

void gameoverInput(Game* game) {
    if (KEY_DOWN_NOW(BUTTON_A)) {
        delay(5);
        game->state = TITLE;
        game->shouldDrawBackground = 1;
        randSeed = 0;
    }
}
