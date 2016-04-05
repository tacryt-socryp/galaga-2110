
#include "myLib.h"
#include "handleDraw.h"
#include "apple.h"
#include <string.h>
#include <stdio.h>

void handleDraw(Game* game) {

    switch(game->state) {
        case TITLE:
            titleDraw(game);
            break;
		case PLAY:
            playDraw(game);
            break;
        case GAMEOVER:
            gameoverDraw(game);
            break;
    }

}

void titleDraw(Game* game) {
    if (game->shouldDrawBackground) {
        fillScreen(MAGENTA);
        game->shouldDrawBackground = 0;
    }
}

void playDraw(Game* game) {
    if (game->shouldDrawBackground) {
        fillScreen(game->backgroundColor);
        game->shouldDrawBackground = 0;
        unsigned short replaceColor = 0x0001f;
        if (game->backgroundColor == GREEN) {
            replaceColor = CYAN;
        } else if (game->backgroundColor == BLACK) {
            replaceColor = GREEN;
        } else if (game->backgroundColor == RED) {
            replaceColor = BLACK;
        }

        for (int i = 0; i < APPLE_HEIGHT * APPLE_WIDTH; i++) {
            if (apple[i] == replaceColor) {
                apple[i] = game->backgroundColor;
            }
        }
        delay(20);
    }

    // draw over old object locations with background color
    for(int i=0; i < game->enemyCount; i++) {
        drawRect(
            game->oldobjs[i].row,
            game->oldobjs[i].col,
            game->oldobjs[i].size,
            game->oldobjs[i].size,
            game->backgroundColor
        );
    }

    // draw over old ship
    drawRect(
        game->oldship.row,
        game->oldship.col,
        game->oldship.size,
        game->oldship.size,
        game->backgroundColor
    );

    // draw shots
    MOVOBJ *oldCur;
    MOVOBJ *cur;

    for (int i = 0; i < 50; i++) {
        oldCur = game->oldshots + i;
        cur = game->shots + i;

        if (oldCur->size != 0) {
            drawRect(
                oldCur->row,
                oldCur->col,
                oldCur->size,
                oldCur->size,
                game->backgroundColor
            );
        }

        if (cur->size != 0) {
            drawRect(cur->row, cur->col, cur->size, cur->size, cur->color);
            *oldCur = *cur;
        }
    }

    // draw text for lives
    drawRect(1, 1, 8, 16, game->backgroundColor);
    char lives[3];
    sprintf(lives, "\x03%d", game->lives);
    drawString(1, 1, lives, WHITE);

    // draw objects
    for(int i=0; i < game->enemyCount; i++) {
        cur = game->objs + i;
        if (cur->type != DEAD) {
            drawRect(cur->row, cur->col, cur->size, cur->size, cur->color);
            game->oldobjs[i] = game->objs[i];
        }
    }

    // draw ship
    cur = &game->ship;
    drawImage3(cur->row, cur->col, APPLE_WIDTH, APPLE_HEIGHT, apple);
    game->oldship = game->ship;

}

void gameoverDraw(Game* game) {
    if (game->shouldDrawBackground) {
        fillScreen(BLUE);
        unsigned short replaceColor = game->backgroundColor;

        for (int i = 0; i < APPLE_HEIGHT * APPLE_WIDTH; i++) {
            if (apple[i] == replaceColor) {
                apple[i] = CYAN;
            }
        }
    }
}

