#include "gui/frontend.h"

int main() {
  srand(time(NULL));
  initGui();
  gameLoop();
  destroyGui();
  return 0;
}
