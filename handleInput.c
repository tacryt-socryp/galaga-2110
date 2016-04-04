
#include "myLib.h"
#include "handleInput.h"
#include "qran_tonk.h"

void moveShip(MOVOBJ* obj, int direction);
int randSeed = 42;

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
        delay(10);
        game->state = TITLE;
        game->shouldDrawBackground = 1;
        game->shipFireRate = 15;
        game->enemyCount = 15;
        game->shotCount = 0;
        game->lives = 3;

        MOVOBJ* cur;
        for (int i = 0; i < 50; i++) {
            cur = game->shots + i;
            cur->size = 0;
            cur = game->oldshots + i;
            cur->size = 0;
        }

        for (int i = 0; i < 15; i++) {
            cur = game->objs + i;
            cur->size = 0;
            cur = game->oldobjs + i;
            cur->size = 0;
        }

        createShip(&game->ship, &game->oldship);

        randSeed = 42;
    }
}
