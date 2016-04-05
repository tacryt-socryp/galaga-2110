
void handleLogic(Game* game);

void titleLogic(Game* game);
void playLogic(Game* game);
void gameoverLogic(Game* game);

void shotCollisionEnemy(Game* game, MOVOBJ *enemy, MOVOBJ *shot);
void shotCollisionShip(Game* game, MOVOBJ *ship, MOVOBJ *shot);
void enemyCollisionShip(Game* game, MOVOBJ *ship, MOVOBJ *enemy);
