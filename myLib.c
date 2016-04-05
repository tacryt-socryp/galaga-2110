#include <stdio.h>
#include <string.h>
#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int r, int c, unsigned short color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

/*  drawimage3
    * A function that will draw an arbitrary sized image
    * onto the screen (with DMA).
    * @param r row to draw the image
    * @param c column to draw the image
    * @param width width of the image
    * @param height height of the image
    * @param image Pointer to the first element of the image.
*/

void drawImage3(int r, int c, int width, int height, unsigned short* image) {

	for (int y = 0; y < height; y++) {
		DMA[3].src = &image[y * width];
        DMA[3].dst = videoBuffer + OFFSET(r + y, c, SCREENWIDTH);
		DMA[3].cnt = width | DMA_ON;
	}
}

void drawRect(int r, int c, int height, int width, unsigned short color) {
    volatile int vColor = color;
    for(int y = 0; y < height; y++) {
        DMA[3].src = &vColor;
        DMA[3].dst = videoBuffer + OFFSET(r + y, c, SCREENWIDTH);
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
    }
}

void fillScreen(unsigned short color) {
    volatile int vColor = color;
    DMA[DMA_CHANNEL_3].src = &vColor;
    DMA[DMA_CHANNEL_3].dst = videoBuffer;
    DMA[DMA_CHANNEL_3].cnt = (SCREENWIDTH * SCREENHEIGHT) |
                             DMA_SOURCE_FIXED |
                             DMA_DESTINATION_INCREMENT |
                             DMA_ON;
}

void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*10000; i++)
	{
		x++;
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

MOVOBJ movobj_new(int row, int col, int rvel, int cvel, int size, u16 color) {
    MOVOBJ a;
    a.row = row;
    a.col = col;
    a.rvel = rvel;
    a.cvel = cvel;
    a.size = size;
    a.color = color;
    a.type = SHIP;
    a.isActive = 0;
    return a;
}

void createShip(MOVOBJ* obj, MOVOBJ* oldobj) {
    obj->row = 140;
    obj->col = 90;
    obj->rvel = 0;
    obj->cvel = 0;
    obj->color = WHITE;
    obj->size = 18;
    obj->type = SHIP;
    oldobj = obj;
}

Game game_new(enum GameState state) {
    Game g;
    g.state = state;
    g.shouldDrawBackground = 1;
    g.shipFireRate = 15;
    g.enemyCount = 15;
    g.deadCount = 15;
    g.shotCount = 0;
    g.lives = 3;
    g.waveNumber = 0;
    g.backgroundColor = CYAN;
    createShip(&g.ship, &g.oldship);
    return g;
}

void createShot(Game* game, int row, int col, int up) {

    int rVel;
    if (up) { // if up is 1, shoot up.
        rVel = -3;
    } else { // if up is 0, shoot down.
        rVel = 3;
    }

    int startRow;
    if (up) {
        startRow = row + 2 * rVel;
    } else {
        startRow = row + rVel;
    }
    MOVOBJ shot = movobj_new(startRow, col, rVel, 0, 3, GRAY);
    if (game->shotCount >= 50) {
        game->shotCount = 0;
    }

    game->shots[game->shotCount] = shot;
    game->oldshots[game->shotCount] = shot;
    game->shotCount++;
}

int collision(MOVOBJ *obj, MOVOBJ *shot) {

    if (obj->col <= shot->col + shot->size &&
            obj->col + obj->size >= shot->col &&
            obj->row <= shot->row + shot->size &&
            obj->row + obj->size >= shot->row) {
        shot->size = 0;
        shot->col = 0;
        shot->row = 0;

        return 1;
    }
    return 0;
}


void drawChar(int row, int col, char ch, u16 color) {
	for(int r=0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel(row+r, col+c, color);
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


