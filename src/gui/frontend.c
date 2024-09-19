#include "frontend.h"

#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <wchar.h>

// Функция для инициализации цветовых пар для GUI
void initColorPairs(void) {
  init_pair(1, COLOR_CYAN, COLOR_CYAN);
  init_pair(2, COLOR_BLUE, COLOR_BLUE);
  init_pair(3, COLOR_RED, COLOR_YELLOW);
  init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(5, COLOR_GREEN, COLOR_GREEN);
  init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(7, COLOR_RED, COLOR_RED);
}

// Инициализация графического пользовательского интерфейса (GUI) для игры
void initGui(void) {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  initColorPairs();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  timeout(READ_DELAY);
}

// Очистка и закрытие GUI, когда игра завершена
void destroyGui(void) {
  printw("\nThe Game is ended. Closing application...\n");
  refresh();
  sleep(1);
  clear();
  refresh();
  endwin();
}

// Основной игровой цикл, который управляет ходом игры
void gameLoop(void) {
  GameParams_t params;
  GameInfo_t info;
  Figure_t figure;
  params.info = &info;
  params.figure = &figure;
  double counter = 0.;

  initializeGameParams(&params);
  getGameParams(&params);

  while (params.isActive) {
    if (counter >= 1.50 - params.info->speed * SPEED_RATE) {
      if (params.state == GAME && !params.info->isPaused) {
        updateGameState();
      }
      counter = 0.;
    }

    counter = counter + READ_DELAY * 1E-3;

    if (params.state == START)
      drawStartScreen(params.info);
    else if (params.state == GAME) {
      drawGui();
      drawInfo(params.info);
      drawField(params.info->field);
    } else if (params.state == GAMEOVER)
      drawGameoverScreen(params.info);

    if (params.info->isPaused) {
      mvprintw(1 + FIELD_SIZE_Y / 2, FIELD_SIZE_X - 1, "isPaused");
      move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
    }

    const int pressedKey = getch();
    const UserAction_t action = getAction(pressedKey);
    if (action != RotateUp) {
      processUserInput(action, false);
    }
  }
}

// Преобразует нажатие клавиш в соответствующие действия пользователя
UserAction_t getAction(const int pressedKey) {
  UserAction_t action = RotateUp;

  if (pressedKey == 10)
    action = Start;
  else if (pressedKey == ' ')
    action = Pause;
  else if (pressedKey == 27)
    action = Terminate;
  else if (pressedKey == KEY_LEFT)
    action = MoveLeft;
  else if (pressedKey == KEY_RIGHT)
    action = MoveRight;
  else if (pressedKey == KEY_DOWN)
    action = MoveDown;
  else if (pressedKey == KEY_UP)
    action = Action;

  return action;
}

// Отображает интерфейс игры, включая границы и разделители
void drawGui(void) {
  clear();

  mvhline(0, 0, ACS_HLINE, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2);
  mvhline(FIELD_SIZE_Y + 1, 0, ACS_HLINE,
          FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2);
  mvhline(FIELD_SIZE_Y - 6, FIELD_SIZE_X * 2 + 2, ACS_HLINE, INFO_SIZE_X * 2);
  mvvline(1, 0, ACS_VLINE, FIELD_SIZE_Y);
  mvvline(1, FIELD_SIZE_X * 2 + 1, ACS_VLINE, FIELD_SIZE_Y);
  mvvline(1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_VLINE, FIELD_SIZE_Y);

  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(0, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_URCORNER);
  mvaddch(FIELD_SIZE_Y + 1, 0, ACS_LLCORNER);
  mvaddch(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2,
          ACS_LRCORNER);
  mvaddch(0, FIELD_SIZE_X * 2 + 1, ACS_TTEE);
  mvaddch(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + 1, ACS_BTEE);

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

// Отображает игровое поле на экране
void drawField(int **field) {
  for (int row = 0; row < FIELD_SIZE_Y; row++)
    for (int col = 0; col < FIELD_SIZE_X; col++)
      if (field[row + 3][col + 3]) {
        attron(COLOR_PAIR(field[row + 3][col + 3]));
        mvaddch(1 + row, 1 + col * 2, ACS_CKBOARD);
        mvaddch(1 + row, 1 + col * 2 + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(field[row + 3][col + 3]));
      }
  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

// Отрисовка информации о текущей игре
void drawInfo(const GameInfo_t *info) {
  mvprintw(2, FIELD_SIZE_X * 2 + 3, "HIGH SCORE: %d", info->highScore);
  mvprintw(4, FIELD_SIZE_X * 2 + 3, "SCORE: %d", info->score);
  mvprintw(6, FIELD_SIZE_X * 2 + 3, "LEVEL: %d", info->level);
  mvprintw(8, FIELD_SIZE_X * 2 + 3, "SPEED: %d", info->speed);
  mvprintw(10, FIELD_SIZE_X * 2 + 3, "Next Figure");
  for (int row = 0; row < (int)kFigureHeight; row++)
    for (int col = 0; col < (int)kFigureWidth; col++) {
      if (info->nextFigure[row][col]) {
        attron(COLOR_PAIR(info->nextFigure[row][col]));
        mvaddch(11 + row, FIELD_SIZE_X * 2 + 6 * 2 + col * 2, ACS_CKBOARD);
        mvaddch(11 + row, FIELD_SIZE_X * 2 + 6 * 2 + col * 2 + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(info->nextFigure[row][col]));
      }
    }

  mvprintw(15, FIELD_SIZE_X * 2 + 3, "SPACE - Pause game");
  mvaddwstr(16, FIELD_SIZE_X * 2 + 5, L"←   - Move left");
  mvaddwstr(17, FIELD_SIZE_X * 2 + 5, L"→   - Move right");
  mvaddwstr(18, FIELD_SIZE_X * 2 + 5, L"↓   - Move down");
  mvaddwstr(19, FIELD_SIZE_X * 2 + 5, L"↑   - Rotate");
  mvaddwstr(20, FIELD_SIZE_X * 2 + 4, L"ESC  - Exit game");
  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

// Отрисовка экрана начала игры
void drawStartScreen(const GameInfo_t *info) {
  drawGui();
  drawInfo(info);

  mvprintw(1 + FIELD_SIZE_Y / 2, 1, "Press ENTER to start");

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

// Отрисовка экрана окончания игры
void drawGameoverScreen(const GameInfo_t *info) {
  drawGui();
  drawInfo(info);
  drawField(info->field);

  mvprintw(FIELD_SIZE_Y / 2, 7, "GAMEOVER");
  mvprintw(FIELD_SIZE_Y / 2 + 1, 5, "Press  ENTER");
  mvprintw(FIELD_SIZE_Y / 2 + 2, 4, "to start again");

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}