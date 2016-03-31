#include <stdio.h>
#include <string.h>
#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

/*  drawimage3
    * A function that will draw an arbitrary sized image
    * onto the screen (with DMA).
    * @param r row to draw the image
    * @param c column to draw the image
    * @param width width of the image
    * @param height height of the image
    * @param image Pointer to the first element of the image.
*/

void drawImage3(int r, int c, int width, int height, const int* image) {

	for (int y = 0; y < height; y++) {
		DMA[3].src = &image[y * (width / 2)];
        DMA[3].dst = videoBuffer + OFFSET(r + y, c, SCREENWIDTH) / 2;
		DMA[3].cnt = (width / 2) | DMA_ON;
	}
}

void drawRect(int r, int c, int height, int width, unsigned short color) {
    volatile int clr = color << 8 | color;
    for(int y = 0; y < height; y++) {
        DMA[3].src = &clr;
        DMA[3].dst = videoBuffer + OFFSET(r + y, c, SCREENWIDTH);
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
    }
}

void fillScreen(unsigned short color) {
    volatile int vColor = color << 8 | color;
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
    return a;
}

void createShip(MOVOBJ* obj, MOVOBJ* oldobj) {
    obj->row = 140;
    obj->col = 90;
    obj->rvel = 0;
    obj->cvel = 0;
    obj->color = WHITE;
    obj->size = 10;
    oldobj = obj;
}

Game game_new(enum GameState state, MOVOBJ objs[1]) {
    Game g;
    g.state = state;
    g.shouldDrawBackground = 1;
    createShip(&g.ship, &g.oldship);
    size_t n = sizeof(objs);
    memcpy(g.objs, objs, n);
    return g;
}
