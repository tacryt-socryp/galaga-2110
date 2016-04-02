// Main program entry
// Name: Logan Allen

#include "myLib.h"
#include "handleInput.h"
#include "handleLogic.h"
#include "handleDraw.h"

int main() {

    // setup
    REG_DISPCTL = MODE3 | BG2_ENABLE; // switch to mode3

    enum GameState state = TITLE;
    Game game = game_new(state);

	while(1) {
        handleInput(&game);
        handleLogic(&game);
		waitForVblank();
        handleDraw(&game);
    }

	return 0;
}
