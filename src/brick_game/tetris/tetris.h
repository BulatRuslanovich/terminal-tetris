#ifndef TETRIS_H
#define TETRIS_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Константы
#define halfPi 1.57
#define kFieldHeight 26
#define kFieldWidth 16
#define kBorderSize 3
#define kFigureHeight 2
#define kFigureWidth 4
#define kPixelEmpty 0

#define kStatesCount 3
#define kSignalsCount 8
#define kFiguresCount 7

#define kLevelMin 1
#define kLevelMax 10
#define kSpeedMin 1
#define kSpeedMax 10

#define kDataFilePath "./score"

#define kLevelThreshold 600

// Очки за линии
#define kScoreRows1 100
#define kScoreRows2 300
#define kScoreRows3 700
#define kScoreRows4 1500

// Повороты фигур

#define ROTATION_MIN 0
#define ROTATION_MAX 3

// Состояние игры
typedef enum {
  START = 0,
  GAME,
  GAMEOVER,
} GameState_t;

// Действия пользователя
typedef enum {
  Start,
  Pause,
  Terminate,
  MoveLeft,
  MoveRight,
  RotateUp,
  MoveDown,
  Action,
} UserAction_t;

// Структуры
typedef struct {
  int **field;
  int **nextFigure;
  int score;
  int highScore;
  int level;
  int speed;
  bool isPaused;
} GameInfo_t;

typedef struct {
  int typeNext;
  int type;
  int rotation;
  int x;
  int y;
} Figure_t;

typedef struct {
  GameInfo_t *info;
  GameState_t state;
  bool isActive;
  Figure_t *figure;
} GameParams_t;

// Прототипы функций
void processUserInput(UserAction_t action, bool hold);
GameInfo_t updateGameState(void);
typedef void (*funcPointer)(GameParams_t *params);
GameParams_t *getGameParams(GameParams_t *params);
void initializeGameParams(GameParams_t *params);
void resetGameField(GameParams_t *params);
void freeGameParams(GameParams_t *params);
void startNewGame(GameParams_t *params);
int generateRandomFigure(int **nextFigure);
void spawnNextFigure(GameParams_t *params);
void moveFigureLeft(GameParams_t *params);
void moveFigureRight(GameParams_t *params);
void rotateFigure(GameParams_t *params);
void moveFigureDown(GameParams_t *params);
void shiftFigureDown(GameParams_t *params);

void attachFigure(GameParams_t *params);
void pauseGame(GameParams_t *params);
void placeFigure(GameParams_t *params);
bool isFigureColliding(GameParams_t *params);
void clearFigureFromField(GameParams_t *params);
int **allocateMatrix(int rows, int cols);

#endif  // TETRIS_H
