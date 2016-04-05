
#include "myLib.h"
#include "handleLogic.h"
#include "qran_tonk.h"

void createObject(MOVOBJ* obj, MOVOBJ* oldobj);
void moveObject(MOVOBJ* obj);
void moveShot(MOVOBJ* obj);
void shootingEnemyLogic(Game* game, MOVOBJ* obj, int waveNumber);
void rammingEnemyLogic(MOVOBJ* obj, int waveNumber);

void nextWave(MOVOBJ* cur, MOVOBJ* old, int i);
void continueWave(Game* game, MOVOBJ* cur);

void handleLogic(Game* game) {

    switch(game->state) {
        case TITLE:
            titleLogic(game);
            break;
		case PLAY:
            playLogic(game);
            break;
        case GAMEOVER:
            gameoverLogic(game);
            break;
    }
 
}

void titleLogic(Game* game) {
    if (game->shouldDrawBackground) {
    }
}

void playLogic(Game* game) {
    MOVOBJ *enemy;
    MOVOBJ *oldEnemy;

    if (game->deadCount == game->enemyCount) {
        // begin new wave
        game->waveNumber++;
        if (game->waveNumber == 1) {
            game->backgroundColor = CYAN;
        } else if (game->waveNumber == 2) {
            game->backgroundColor = GREEN;
        } else if (game->waveNumber == 3) {
            game->backgroundColor = BLACK;
        } else if (game->waveNumber == 4) {
            game->backgroundColor = RED;
        }
        game->deadCount = 0;
        game->enemyCount = game->waveNumber * 5;
        game->shouldDrawBackground = 1;
        for (int i = 0; i < game->enemyCount; i++) {
            enemy = game->objs + i;
            oldEnemy = game->oldobjs + i;

            nextWave(enemy, oldEnemy, i);
        }
    } else {
        for (int i = 0; i < game->enemyCount; i++) {
            enemy = game->objs + i;

            continueWave(game, enemy);
        }
    }


    MOVOBJ *shot;
    for (int i = 0; i < 50; i++) {
        shot = game->shots + i;
        
        if (shot->size != NULL) {
            moveShot(shot);

            for (int e = 0; e < game->enemyCount; e++) {
                enemy = game->objs + e;
                if (enemy->size != NULL) {
                    shotCollisionEnemy(game, enemy, shot);
                }
            }

            shotCollisionShip(game, &game->ship, shot);
        }

    }
}

void nextWave(MOVOBJ* cur, MOVOBJ* old, int i) {
    createObject(cur, old);
    if (i % 2 == 0) {
        cur->type = SHOOTENEMY;
    } else {
        cur->type = RAMENEMY;
    }
}

void continueWave(Game *game, MOVOBJ* cur) {
    switch (cur->type) {
    case DEAD:
        // do nothing
        break;
    case SHIP:
        // this won't happen
        break;
    case SHOOTENEMY:
        shootingEnemyLogic(game, cur, game->waveNumber);
        break;
    case RAMENEMY:
        enemyCollisionShip(game, &game->ship, cur);
        rammingEnemyLogic(cur, game->waveNumber);
        break;
    }
}

void shootingEnemyLogic(Game* game, MOVOBJ* obj, int waveNumber) {
    int shouldNotShoot = (qran() * (300 - (waveNumber * 50)) >> 15) + 0;
    int DOWN = 0;
    if (shouldNotShoot == 0) {
        createShot(game, obj->row + obj->size + 1, obj->col, DOWN);
    }

    int shouldChangeDirection = (qran() * (300 - (waveNumber * 50)) >> 15) + 0;
    if (shouldChangeDirection == 0) {
        int velocityRange = waveNumber / 2 + 1;
        obj->cvel = (qran() * (2 * velocityRange) >> 15) - (velocityRange);
    }
    moveObject(obj);
}

void rammingEnemyLogic(MOVOBJ* obj, int waveNumber) {

    if (obj->isActive == 0) {
        if (obj->row > 40) {
            // possibility to activate ram if not active now
            int shouldNotRam = (qran() * (700 - (waveNumber * 50)) >> 15) + 0;
            obj->isActive = shouldNotRam == 0;
        }

        if (obj->row > 50) {
            // normal state
            obj->rvel = 0;
            int shouldChangeDirection = (qran() * (300 - (waveNumber * 50)) >> 15) + 0;
            if (shouldChangeDirection == 0) {
                obj->cvel = (qran() * (1 + 1) >> 15) - 1;
            }
        }
    } else {
        if (obj->rvel != 3) {
            // begin ram action
            obj->rvel = 3;
            obj->cvel = (qran() * (2 + 2) >> 15) - 2;
        } else if (obj->row < 20) {
            // deactivate ram action when object returns
            obj->rvel = 2;
            obj->isActive = 0;
        }
    }

    moveObject(obj);
}

void createObject(MOVOBJ* obj, MOVOBJ* oldobj) {
    obj->row = (qran() * (20) >> 15) + 50; // between 50 and 70
    obj->col = (qran() * (60) >> 15) + 80; // between 80 and 140
    obj->rvel = 0;
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
        obj->row = 0;
        // obj->row = 159-obj->size+1;
        // obj->rvel = -obj->rvel;
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

void moveShot(MOVOBJ* shot) {
    shot->row += shot->rvel;
    shot->col += shot->cvel;

    if ((shot->row < 0) || (shot->row > 159 - shot->size + 1) ||
            (shot->col < 0) || (shot->col > 239 - shot->size + 1)) {
        shot->rvel = 0;
        shot->cvel = 0;
        shot->row = 0;
        shot->size = 0;
    }
}

void shotCollisionEnemy(Game* game, MOVOBJ *enemy, MOVOBJ *shot) {
    int collide = collision(enemy, shot);

    if (collide && shot->rvel < 0) {
        enemy->size = 0;
        enemy->type = DEAD;
        game->deadCount++;
        game->score++;
    }

}

void shotCollisionShip(Game* game, MOVOBJ *ship, MOVOBJ *shot) {
    int collide = collision(ship, shot);
    if (collide) {
        game->lives--;
        if (game->lives <= 0) {
            game->state = GAMEOVER;
            game->shouldDrawBackground = 1;
        }
    }

}

void enemyCollisionShip(Game* game, MOVOBJ *ship, MOVOBJ *enemy) {
    int collide = collision(ship, enemy);
    if (collide) {
        game->lives--;
        enemy->size = 0;
        enemy->type = DEAD;
        game->deadCount++;
        game->score++;
        if (game->lives <= 0) {
            game->state = GAMEOVER;
            game->shouldDrawBackground = 1;
        }
    }

}

void gameoverLogic(Game* game) {
    if (game->shouldDrawBackground) {
    }
}
