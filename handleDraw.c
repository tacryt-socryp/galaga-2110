
#include "myLib.h"
#include "handleDraw.h"
#include <string.h>

void handleDraw(Game game) {

    switch(game.state) {
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

void titleDraw(Game game) {
    if (game.shouldDrawBackground == 1) {
        fillScreen(MAGENTA);
        game.shouldDrawBackground = 0;
    }
}

void playDraw(Game game) {
    if (game.shouldDrawBackground == 1) {
        fillScreen(CYAN);
        game.shouldDrawBackground = 0;
    }

    for(int i=0; i<1; i++) {
        drawRect(
            game.oldobjs[i].row,
            game.oldobjs[i].col,
            game.oldobjs[i].size,
            game.oldobjs[i].size,
            CYAN
        );
    }

    MOVOBJ *cur;
    for(int i=0; i<1; i++) {
        cur = game.objs+i;
        drawRect(cur->row, cur->col, cur->size, cur->size, cur->color);
        game.oldobjs[i] = game.objs[i];
    }

    cur = &game.ship;
    drawRect(cur->row, cur->col, cur->size, cur->size, cur->color);
    game.oldship = game.ship;
}

void gameoverDraw(Game game) {
    if (game.shouldDrawBackground) {
        fillScreen(GRAY);
        game.shouldDrawBackground = 0;
    }
}


void drawChar(int row, int col, char ch, u16 color) {

	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 6; c++) {
			if (fontdata_6x8[ch * 48 + OFFSET(r,c,6)]) {
                drawRect(row + r, col + c, 1, 1, color);
			}
		}
	}
}


void drawString(int row, int col, char *str, u16 color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}
