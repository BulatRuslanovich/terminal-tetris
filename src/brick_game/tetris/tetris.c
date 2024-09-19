
#include "tetris.h"

// Таблица конечного автомата (FSM) с действиями, зависящими от состояния и
// сигнала пользователя
funcPointer fsmTable[3][8] = {
    {startNewGame, NULL, freeGameParams, NULL, NULL, NULL, NULL,
     NULL},  // Start
    {NULL, pauseGame, freeGameParams, moveFigureLeft, moveFigureRight, NULL,
     moveFigureDown, rotateFigure},  // Game
    {startNewGame, NULL, freeGameParams, NULL, NULL, NULL, NULL,
     NULL}  // Gameover
};

// Массив фигур тетриса, каждая фигура описана 4 парами координат
int tetrisFigures[7][8] = {
    {0, -1, 0, 0, 0, 1, 0, 2},    // Прямая
    {-1, -1, 0, -1, 0, 0, 0, 1},  // Фигура "Т"
    {0, -1, 0, 0, 0, 1, -1, 1},   // Фигура "Зигзаг"
    {-1, 0, -1, 1, 0, 0, 0, 1},   // Обратный "Зигзаг"
    {0, -1, 0, 0, -1, 0, -1, 1},  // Квадрат
    {0, -1, 0, 0, -1, 0, 0, 1},   // "Г"-образная
    {-1, -1, -1, 0, 0, 0, 0, 1},  // Обратная "Г"-образная
};

// Обработка ввода пользователя и запуск соответствующей функции в зависимости
// от состояния игры
void processUserInput(UserAction_t action, bool hold) {
  if (hold) printf(" ");
  GameParams_t *params = getGameParams(NULL);
  GameState_t state = params->state;
  funcPointer func = fsmTable[state][action];
  if (func) func(params);
}

// Обновление текущего состояния игры
GameInfo_t updateGameState(void) {
  GameParams_t *params = getGameParams(NULL);
  shiftFigureDown(params);
  return *params->info;
}

// Получение или установка текущих параметров игры
GameParams_t *getGameParams(GameParams_t *params) {
  static GameParams_t *info;
  if (params != NULL) info = params;
  return info;
}

// Инициализация параметров игры, выделение памяти для поля и фигуры
void initializeGameParams(GameParams_t *params) {
  params->info->field = allocateMatrix(kFieldHeight, kFieldWidth);
  resetGameField(params);
  params->info->nextFigure = allocateMatrix(kFigureHeight, kFigureWidth);
  params->info->score = 0;

  FILE *fp = fopen(kDataFilePath, "r");
  if (!fp) {
    params->info->highScore = 0;
    fp = fopen(kDataFilePath, "w");
    fprintf(fp, "0\n");
  } else {
    int highScore;
    fscanf(fp, "%d\n", &highScore);
    params->info->highScore = highScore;
  }
  fclose(fp);

  params->info->level = kLevelMin;
  params->info->speed = kSpeedMin;
  params->info->isPaused = 0;
  params->figure->typeNext = generateRandomFigure(params->info->nextFigure);
  params->state = START;
  params->isActive = true;
}

// Очистка игрового поля
void resetGameField(GameParams_t *params) {
  for (int row = 0; row < kFieldHeight; row++) {
    for (int col = 0; col < kFieldWidth; col++) {
      params->info->field[row][col] =
          row > kFieldHeight - kBorderSize - 1 || col < kBorderSize ||
                  col > kFieldWidth - kBorderSize - 1
              ? 1
              : 0;
    }
  }
}

// Освобождение памяти, выделенной под параметры игры
void freeGameParams(GameParams_t *params) {
  if (params->info->field) {
    for (size_t rowIdx = 0; rowIdx < kFieldHeight; rowIdx++) {
      free(params->info->field[rowIdx]);
      params->info->field[rowIdx] = NULL;
    }
    free(params->info->field);
    params->info->field = NULL;
  }

  if (params->info->nextFigure) {
    for (size_t rowIdx = 0; rowIdx < kFigureHeight; rowIdx++) {
      free(params->info->nextFigure[rowIdx]);
      params->info->nextFigure[rowIdx] = NULL;
    }
    free(params->info->nextFigure);
    params->info->nextFigure = NULL;
  }

  params->state = GAMEOVER;
  params->isActive = false;
}

// Генерация случайной фигуры для следующего хода
int generateRandomFigure(int **nextFigure) {
  int type = rand() % kFiguresCount;

  for (int row = 0; row < kFigureHeight; row++) {
    for (int col = 0; col < kFigureWidth; col++) {
      nextFigure[row][col] = 0;
    }
  }

  for (int i = 1; i < 8; i += 2) {
    nextFigure[tetrisFigures[type][i - 1] + 1][tetrisFigures[type][i] + 1] =
        type + 1;
  }

  return type;
}

// Начало новой игры, инициализация параметров
void startNewGame(GameParams_t *params) {
  resetGameField(params);
  FILE *fp = fopen(kDataFilePath, "r");
  int highScore;
  fscanf(fp, "%d\n", &highScore);
  params->info->highScore = highScore;
  fclose(fp);
  params->info->score = 0;
  params->info->level = kLevelMin;
  params->info->speed = kSpeedMin;
  params->state = GAME;
  spawnNextFigure(params);
}

// Появление новой фигуры на игровом поле
void spawnNextFigure(GameParams_t *params) {
  params->figure->type = params->figure->typeNext;
  params->figure->x = kFieldWidth / 2;
  params->figure->y = 2;
  params->figure->rotation = 0;
  params->figure->typeNext = generateRandomFigure(params->info->nextFigure);
  placeFigure(params);
}

// Добавление фигуры на поле
void placeFigure(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;
  int rotation = params->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = round(tetrisFigures[type][i] * cos(halfPi * rotation) +
                   tetrisFigures[type][i - 1] * sin(halfPi * rotation));
    int yy = (int)round(-tetrisFigures[type][i] * sin(halfPi * rotation) +
                        tetrisFigures[type][i - 1] * cos(halfPi * rotation));
    params->info->field[yy + y][xx + x] = type + 1;
  }
}

// Проверка на столкновение фигуры с границами или другими фигурами
bool isFigureColliding(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;
  int rotation = params->figure->rotation;

  bool isColliding = false;
  for (int i = 1; i < 8 && !isColliding; i += 2) {
    int xx = (int)round(tetrisFigures[type][i] * cos(halfPi * rotation) +
                        tetrisFigures[type][i - 1] * sin(halfPi * rotation));
    int yy = (int)round(-tetrisFigures[type][i] * sin(halfPi * rotation) +
                        tetrisFigures[type][i - 1] * cos(halfPi * rotation));
    if (params->info->field[yy + y][xx + x]) isColliding = true;
  }

  return !isColliding;
}

// Очистка фигуры с игрового поля
void clearFigureFromField(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;
  int rotation = params->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(tetrisFigures[type][i] * cos(halfPi * rotation) +
                        tetrisFigures[type][i - 1] * sin(halfPi * rotation));
    int yy = (int)round(-tetrisFigures[type][i] * sin(halfPi * rotation) +
                        tetrisFigures[type][i - 1] * cos(halfPi * rotation));
    params->info->field[yy + y][xx + x] = 0;
  }
}

// Сдвиг фигуры вниз на игровом поле
void shiftFigureDown(GameParams_t *params) {
  clearFigureFromField(params);
  params->figure->y++;
  bool canShift = isFigureColliding(params);

  if (!canShift) params->figure->y--;

  placeFigure(params);

  if (!canShift) attachFigure(params);
}

// Присоединение фигуры к полю и удаление заполненных рядов
void attachFigure(GameParams_t *params) {
  int rows = 0;
  for (int row = kFieldHeight - kBorderSize - 1; row > 2; row--) {
    bool cycle = true;
    while (cycle) {
      int rowBlocks = 0;
      for (int col = 3; col < kFieldWidth - 3; col++) {
        if (params->info->field[row][col]) rowBlocks++;
      }

      if (rowBlocks == kFieldWidth - 6) {
        rows++;
        for (int i = row; i > 1; i--) {
          for (int col = kBorderSize; col < kFieldWidth - kBorderSize; col++) {
            params->info->field[i][col] = params->info->field[i - 1][col];
          }
        }
      } else
        cycle = false;
    }
  }

  if (rows == 1)
    params->info->score += kScoreRows1;
  else if (rows == 2)
    params->info->score += kScoreRows2;
  else if (rows == 3)
    params->info->score += kScoreRows3;
  else if (rows >= 4)
    params->info->score += kScoreRows4;

  if (params->info->score > params->info->highScore) {
    params->info->highScore = params->info->score;
    FILE *fp = fopen(kDataFilePath, "w");
    fprintf(fp, "%d\n", params->info->highScore);
    fclose(fp);
  }

  params->info->level = params->info->score / kLevelThreshold + 1 <= kLevelMax
                            ? params->info->score / kLevelThreshold + 1
                            : kLevelMax;
  params->info->speed = params->info->level;

  spawnNextFigure(params);
  clearFigureFromField(params);
  params->figure->y++;

  if (!isFigureColliding(params)) {
    params->figure->y--;
    params->state = GAMEOVER;
  }

  placeFigure(params);
}

// Перемещение фигуры влево
void moveFigureLeft(GameParams_t *params) {
  if (!params->info->isPaused) {
    clearFigureFromField(params);
    params->figure->x--;

    if (!isFigureColliding(params)) params->figure->x++;

    placeFigure(params);
  }
}

// Перемещение фигуры вправо
void moveFigureRight(GameParams_t *params) {
  if (!params->info->isPaused) {
    clearFigureFromField(params);
    params->figure->x++;

    if (!isFigureColliding(params)) params->figure->x--;

    placeFigure(params);
  }
}

// Перемещение фигуры вниз до упора
void moveFigureDown(GameParams_t *params) {
  if (!params->info->isPaused) {
    clearFigureFromField(params);
    bool canMove = true;
    while (canMove) {
      params->figure->y++;
      canMove = isFigureColliding(params);

      if (!canMove) params->figure->y--;
    }

    placeFigure(params);
    attachFigure(params);
  }
}

// Поворот фигуры
void rotateFigure(GameParams_t *params) {
  if (!params->info->isPaused) {
    clearFigureFromField(params);
    params->figure->rotation = (params->figure->rotation + 1 <= ROTATION_MAX)
                                   ? params->figure->rotation + 1
                                   : ROTATION_MIN;

    if (!isFigureColliding(params)) {
      params->figure->rotation = (params->figure->rotation - 1 >= ROTATION_MIN)
                                     ? params->figure->rotation - 1
                                     : ROTATION_MAX;
    }

    placeFigure(params);
  }
}

// Переключение состояния паузы игры
void pauseGame(GameParams_t *params) {
  params->info->isPaused = !params->info->isPaused;
}

// Выделение памяти для двумерного массива
int **allocateMatrix(int rows, int cols) {
  int **matrix = (int **)calloc(rows, sizeof(int *));
  for (size_t rowIdx = 0; rowIdx < (size_t)rows; rowIdx++) {
    matrix[rowIdx] = (int *)calloc(cols, sizeof(int));
  }

  return matrix;
}