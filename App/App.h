//
// Created by Paul on 16.03.2025.
//

#ifndef GAME_ENGINE__APP_H_
#define GAME_ENGINE__APP_H_

#include "../Window/Window.h"
class App {
 public:
  App();
  int Go();
 private:
  void DoFrame();
 private:
  Window wnd
};

#endif //GAME_ENGINE__APP_H_
