//
// Created by Paul on 16.03.2025.
//

#ifndef GAME_ENGINE__TIMER_H_
#define GAME_ENGINE__TIMER_H_
#pragma once
#include <chrono>
#include <iostream>

class Timer {
 public:
  Timer();
  float Mark();
  float Peek();
 private:
  std::chrono::steady_clock::time_point last;
};

#endif //GAME_ENGINE__TIMER_H_
