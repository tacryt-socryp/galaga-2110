// Additional definitions

#define SCREENWIDTH 240
#define SCREENHEIGHT 160

#ifndef NULL
#define NULL 0
#endif


typedef unsigned short u16;

/* Variables */
enum GameState {
    TITLE,
    PLAY,
    GAMEOVER
};

enum ObjType {
    DEAD,
    SHIP,
    SHOOTENEMY,
    RAMENEMY
};

typedef struct
{
	int row;
	int col;
	int rvel;
	int cvel;
	int size;

    enum ObjType type;
    int isActive;    

	u16 color;
} MOVOBJ;

typedef struct {
    enum GameState state;
    u16 shouldDrawBackground;
    int backgroundColor;

    int lives;
    MOVOBJ ship;
    MOVOBJ oldship;

    int score;
    int shipFireRate;
    int shotRateLimiter;
    int shotCount;
    MOVOBJ shots[50];
    MOVOBJ oldshots[50];

    int waveNumber;
    int enemyCount;
    int deadCount;
    MOVOBJ objs[100];
    MOVOBJ oldobjs[100];
} Game;

// defaults

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006 

extern unsigned short *videoBuffer;

#define COLOR(r, g, b)  ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define YELLOW COLOR(31,31,0)
#define CYAN COLOR(0, 31, 31)
#define MAGENTA COLOR(31,0,31)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define GRAY COLOR(15,15,15)
#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */

#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(const volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)


// Prototypes
void setPixel(int r, int c, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();

MOVOBJ movobj_new(int row, int col, int rvel, int cvel, int size, u16 color);
Game game_new(enum GameState state);

void drawImage3(int r, int c, int width, int height, const unsigned short* image);
void drawImageNotConst(int r, int c, int width, int height, unsigned short* image);
void drawRect(int r, int c, int height, int width, unsigned short color);
void fillScreen(unsigned short color);
void createShot(Game* game, int row, int col, int up);
int collision(MOVOBJ *obj, MOVOBJ *shot);
void createShip(MOVOBJ* obj, MOVOBJ* oldobj);

extern const unsigned char fontdata_6x8[12288];

void drawChar(int row, int col, char ch, u16 color);
void drawString(int row, int col, char *str, u16 color);


