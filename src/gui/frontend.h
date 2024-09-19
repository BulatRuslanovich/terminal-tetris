#ifndef FRONTEND_H
#define FRONTEND_H

#include "../brick_game/tetris/tetris.h"

#define FRAME_RATE 60
#define READ_DELAY 50
#define FIELD_SIZE_X 10
#define FIELD_SIZE_Y 20
#define INFO_SIZE_X 10
#define INFO_SIZE_Y 20
#define GUI_SCALE 1
#define SPEED_RATE 0.15

void initColorPairs(void);

void initGui(void);

void destroyGui(void);

void gameLoop(void);

UserAction_t getAction(int pressedKey);

void drawGui(void);

void drawField(int **field);

void drawInfo(const GameInfo_t *info);

void drawStartScreen(const GameInfo_t *info);

void drawGameoverScreen(const GameInfo_t *info);
#endif  // FRONTEND_H
