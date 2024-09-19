#include "front_test.h"

// userInput

// updateGameState
START_TEST(tc_ok_1) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  getGameParams(&params);
  spawnNextFigure(&params);
  GameInfo_t infoCheck = updateGameState();

  ck_assert_ptr_eq(params.info->field, infoCheck.field);
  ck_assert_ptr_eq(params.info->nextFigure, infoCheck.nextFigure);
  ck_assert_int_eq(params.info->highScore, infoCheck.highScore);
  ck_assert_int_eq(params.info->score, infoCheck.score);
  ck_assert_int_eq(params.info->level, infoCheck.level);
  ck_assert_int_eq(params.info->isPaused, infoCheck.isPaused);
  ck_assert_int_eq(params.info->speed, infoCheck.speed);
  freeGameParams(&params);
}
END_TEST

// getGameParams
START_TEST(tc_ok_2) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  GameParams_t *paramsCheck1 = getGameParams(&params);
  GameParams_t *paramsCheck2 = getGameParams(NULL);

  ck_assert_ptr_eq(&params, paramsCheck1);
  ck_assert_ptr_eq(&params, paramsCheck2);
  freeGameParams(&params);
}
END_TEST

// initializeGameParams
START_TEST(tc_ok_3) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);

  ck_assert_int_eq(params.info->score, 0);
  ck_assert_int_eq(params.info->level, 1);
  ck_assert_int_eq(params.info->speed, 1);
  ck_assert_int_eq(params.info->isPaused, 0);
  ck_assert_int_ge(params.figure->typeNext, 0);
  ck_assert_int_le(params.figure->typeNext, 6);
  ck_assert_int_eq(params.state, START);
  ck_assert_int_eq(params.isActive, true);
  freeGameParams(&params);
}
END_TEST

// freeGameParams
START_TEST(tc_ok_4) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  freeGameParams(&params);

  ck_assert_ptr_null(params.info->field);
  ck_assert_ptr_null(params.info->nextFigure);
  ck_assert_int_eq(params.state, GAMEOVER);
  ck_assert_int_eq(params.isActive, false);
}
END_TEST

START_TEST(tc_ok_5) {
  GameParams_t params;
  GameInfo_t info;
  params.info = &info;
  params.info->field = NULL;
  params.info->nextFigure = NULL;

  freeGameParams(&params);

  ck_assert_ptr_null(params.info->field);
  ck_assert_ptr_null(params.info->nextFigure);
  ck_assert_int_eq(params.state, GAMEOVER);
  ck_assert_int_eq(params.isActive, false);
}
END_TEST

// resetGameField
START_TEST(tc_ok_6) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  startNewGame(&params);

  ck_assert_int_eq(params.info->score, 0);
  ck_assert_int_eq(params.info->level, 1);
  ck_assert_int_eq(params.info->speed, 1);
  ck_assert_int_eq(params.state, GAME);
  ck_assert_int_eq(params.isActive, true);
  freeGameParams(&params);
}
END_TEST

// startNewGame
START_TEST(tc_ok_7) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.info->field[kFieldHeight / 2][kFieldWidth / 2] = 1;
  resetGameField(&params);

  bool isFieldEqual = true;
  for (int row = 0; row < kFieldHeight && isFieldEqual; row++)
    for (int col = 0; col < kFieldWidth && isFieldEqual; col++) {
      int checkValue = (row > 22 || col < 3 || col > 12) ? 1 : 0;
      isFieldEqual = params.info->field[row][col] == checkValue;
    }
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

// generateRandomFigure
START_TEST(tc_ok_8) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->typeNext = generateRandomFigure(params.info->nextFigure);

  ck_assert_int_ge(params.figure->typeNext, 0);
  ck_assert_int_le(params.figure->typeNext, 6);
  freeGameParams(&params);
}
END_TEST

// spawnNextFigure
START_TEST(tc_ok_9) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->typeNext = 0;
  spawnNextFigure(&params);

  ck_assert_int_eq(params.figure->type, 0);
  ck_assert_int_eq(params.figure->y, 2);
  ck_assert_int_eq(params.figure->x, kFieldWidth / 2);
  ck_assert_int_eq(params.figure->rotation, 0);
  freeGameParams(&params);
}
END_TEST

// moveFigureLeft
START_TEST(tc_ok_10) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  moveFigureLeft(&params);

  ck_assert_int_ne(params.figure->x, previousX);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_11) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  params.info->isPaused = 1;
  moveFigureLeft(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_12) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = 4;
  int previousX = params.figure->x;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  moveFigureLeft(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  freeGameParams(&params);
}
END_TEST

// moveFigureRight
START_TEST(tc_ok_13) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  moveFigureRight(&params);

  ck_assert_int_ne(params.figure->x, previousX);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_14) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  int previousX = params.figure->x;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  params.info->isPaused = 1;
  moveFigureRight(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_15) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth - 6;
  int previousX = params.figure->x;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  moveFigureRight(&params);

  ck_assert_int_eq(params.figure->x, previousX);
  freeGameParams(&params);
}
END_TEST

// rotateFigure
START_TEST(tc_ok_16) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  params.figure->rotation = 0;
  params.figure->type = 0;  // I figure
  int previousRotation = params.figure->rotation;
  rotateFigure(&params);

  ck_assert_int_ne(params.figure->rotation, previousRotation);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_17) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  int previousRotation = params.figure->rotation;
  params.info->isPaused = 1;
  rotateFigure(&params);

  ck_assert_int_eq(params.figure->rotation, previousRotation);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_18) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth - 3;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  int previousRotation = params.figure->rotation;
  rotateFigure(&params);

  ck_assert_int_eq(params.figure->rotation, previousRotation);
  freeGameParams(&params);
}
END_TEST

// moveFigureDown
START_TEST(tc_ok_19) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  int previousY = params.figure->y;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  moveFigureDown(&params);

  bool isFieldEqual = true;
  isFieldEqual = params.info->field[params.figure->y][previousY] == 0;
  isFieldEqual = params.info->field[params.figure->y][previousY] == 0;
  isFieldEqual = params.info->field[params.figure->y][previousY] == 0;
  isFieldEqual = params.info->field[params.figure->y][previousY] == 0;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 - 1] == 1;
  isFieldEqual = params.info->field[kFieldHeight - 4][kFieldHeight / 2] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 + 1] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_20) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  params.info->isPaused = 1;
  moveFigureDown(&params);

  bool isFieldEqual = true;
  isFieldEqual =
      params.info->field[kFieldHeight / 2][kFieldHeight / 2 - 1] == 1;
  isFieldEqual = params.info->field[kFieldHeight / 2][kFieldHeight / 2] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight / 2][kFieldHeight / 2 + 1] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight / 2][kFieldHeight / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_21) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight - 4;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  int previousY = params.figure->y;
  moveFigureDown(&params);

  bool isFieldEqual = true;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 - 1] == 1;
  isFieldEqual = params.info->field[kFieldHeight - 4][kFieldHeight / 2] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 + 1] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

// shiftFigureDown
START_TEST(tc_ok_22) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  int previousY = params.figure->y;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  shiftFigureDown(&params);

  ck_assert_int_ne(params.figure->y, previousY);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_23) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight - 4;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  int previousY = params.figure->y;
  shiftFigureDown(&params);

  bool isFieldEqual = true;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual = params.info->field[previousY][params.figure->x] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 - 1] == 1;
  isFieldEqual = params.info->field[kFieldHeight - 4][kFieldHeight / 2] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 + 1] == 1;
  isFieldEqual =
      params.info->field[kFieldHeight - 4][kFieldHeight / 2 + 2] == 1;
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

// attachFigure
START_TEST(tc_ok_24) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  for (int col = 0; col < kFieldWidth; col++)
    params.info->field[kFieldHeight - 4][col] = 1;
  attachFigure(&params);

  ck_assert_int_eq(params.info->score, 100);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_25) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  for (int col = 0; col < kFieldWidth; col++) {
    params.info->field[kFieldHeight - 4][col] = 1;
    params.info->field[kFieldHeight - 5][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.info->score, 300);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_26) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  for (int col = 0; col < kFieldWidth; col++) {
    params.info->field[kFieldHeight - 4][col] = 1;
    params.info->field[kFieldHeight - 5][col] = 1;
    params.info->field[kFieldHeight - 6][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.info->score, 700);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_27) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  for (int col = 0; col < kFieldWidth; col++) {
    params.info->field[kFieldHeight - 4][col] = 1;
    params.info->field[kFieldHeight - 5][col] = 1;
    params.info->field[kFieldHeight - 6][col] = 1;
    params.info->field[kFieldHeight - 7][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.info->score, 1500);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_28) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.info->score = 6000;
  for (int col = 0; col < kFieldWidth; col++) {
    params.info->field[kFieldHeight - 4][col] = 1;
    params.info->field[kFieldHeight - 5][col] = 1;
    params.info->field[kFieldHeight - 6][col] = 1;
    params.info->field[kFieldHeight - 7][col] = 1;
  }
  attachFigure(&params);

  ck_assert_int_eq(params.info->score, 7500);
  ck_assert_int_eq(params.info->level, 10);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_29) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.info->field[3][kFieldWidth / 2] = 1;
  attachFigure(&params);

  ck_assert_int_eq(params.state, GAMEOVER);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_30) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.info->score = params.info->highScore + 100;
  attachFigure(&params);

  ck_assert_int_eq(params.info->score, params.info->highScore);
  freeGameParams(&params);
}
END_TEST

// placeFigure
START_TEST(tc_ok_31) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  placeFigure(&params);

  bool isFieldEqual = true;
  isFieldEqual = params.info->field[params.figure->y][params.figure->x - 1] ==
                 params.figure->type + 1;
  isFieldEqual = params.info->field[params.figure->y][params.figure->x] ==
                 params.figure->type + 1;
  isFieldEqual = params.info->field[params.figure->y][params.figure->x + 1] ==
                 params.figure->type + 1;
  isFieldEqual = params.info->field[params.figure->y][params.figure->x + 2] ==
                 params.figure->type + 1;
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

// isFigureColliding
START_TEST(tc_ok_32) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  bool isNotCollide = isFigureColliding(&params);

  ck_assert_int_eq(isNotCollide, true);
  freeGameParams(&params);
}
END_TEST

START_TEST(tc_ok_33) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight - 3;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  bool isNotCollide = isFigureColliding(&params);

  ck_assert_int_eq(isNotCollide, false);
  freeGameParams(&params);
}
END_TEST

// clearFigureFromField
START_TEST(tc_ok_34) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  params.figure->y = kFieldHeight / 2;
  params.figure->x = kFieldWidth / 2;
  params.figure->type = 0;  // I figure
  params.figure->rotation = 0;
  clearFigureFromField(&params);

  bool isFieldEqual = true;
  isFieldEqual =
      params.info->field[params.figure->y][params.figure->x - 1] == 0;
  isFieldEqual = params.info->field[params.figure->y][params.figure->x] == 0;
  isFieldEqual =
      params.info->field[params.figure->y][params.figure->x + 1] == 0;
  isFieldEqual =
      params.info->field[params.figure->y][params.figure->x + 2] == 0;
  ck_assert_int_eq(isFieldEqual, true);
  freeGameParams(&params);
}
END_TEST

// allocateMatrix
START_TEST(tc_ok_35) {
  GameParams_t params;
  GameInfo_t info;
  info.field = NULL;
  info.nextFigure = NULL;
  params.info = &info;

  info.field = allocateMatrix(kFieldHeight, kFieldWidth);

  ck_assert_ptr_nonnull(params.info->field);
  freeGameParams(&params);
}
END_TEST

// isPaused
START_TEST(tc_ok_36) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;

  initializeGameParams(&params);
  pauseGame(&params);

  ck_assert_int_eq(params.info->isPaused, 1);
  freeGameParams(&params);
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("tetris");
  TCase *tcases_ok;

  tcases_ok = tcase_create("tetris_ok");
  tcase_add_test(tcases_ok, tc_ok_1);
  tcase_add_test(tcases_ok, tc_ok_2);
  tcase_add_test(tcases_ok, tc_ok_3);
  tcase_add_test(tcases_ok, tc_ok_4);
  tcase_add_test(tcases_ok, tc_ok_5);
  tcase_add_test(tcases_ok, tc_ok_6);
  tcase_add_test(tcases_ok, tc_ok_7);
  tcase_add_test(tcases_ok, tc_ok_8);
  tcase_add_test(tcases_ok, tc_ok_9);
  tcase_add_test(tcases_ok, tc_ok_10);
  tcase_add_test(tcases_ok, tc_ok_11);
  tcase_add_test(tcases_ok, tc_ok_12);
  tcase_add_test(tcases_ok, tc_ok_13);
  tcase_add_test(tcases_ok, tc_ok_14);
  tcase_add_test(tcases_ok, tc_ok_15);
  tcase_add_test(tcases_ok, tc_ok_16);
  tcase_add_test(tcases_ok, tc_ok_17);
  tcase_add_test(tcases_ok, tc_ok_18);
  tcase_add_test(tcases_ok, tc_ok_19);
  tcase_add_test(tcases_ok, tc_ok_20);
  tcase_add_test(tcases_ok, tc_ok_21);
  tcase_add_test(tcases_ok, tc_ok_22);
  tcase_add_test(tcases_ok, tc_ok_23);
  tcase_add_test(tcases_ok, tc_ok_24);
  tcase_add_test(tcases_ok, tc_ok_25);
  tcase_add_test(tcases_ok, tc_ok_26);
  tcase_add_test(tcases_ok, tc_ok_27);
  tcase_add_test(tcases_ok, tc_ok_28);
  tcase_add_test(tcases_ok, tc_ok_29);
  tcase_add_test(tcases_ok, tc_ok_30);
  tcase_add_test(tcases_ok, tc_ok_31);
  tcase_add_test(tcases_ok, tc_ok_32);
  tcase_add_test(tcases_ok, tc_ok_33);
  tcase_add_test(tcases_ok, tc_ok_34);
  tcase_add_test(tcases_ok, tc_ok_35);
  tcase_add_test(tcases_ok, tc_ok_36);

  suite_add_tcase(s, tcases_ok);

  return s;
}