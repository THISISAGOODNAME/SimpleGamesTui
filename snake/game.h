#ifndef MINESWEEPER_GAME
#define MINESWEEPER_GAME

#include "board.h"
#include <chrono>

namespace snake {

enum class Direction { up, down, left, right };

class Game
{
  struct Point
  {
    int x, y;
  };

  std::vector<Point> snakeSegments;

  Point foodPosition;

  std::vector<Direction> directionQueue = { Direction::right };

  bool snake_alive = false;

  Board board;

  int time;
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> start_time{};

  static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> time_now();
  [[nodiscard]] std::chrono::seconds elapsed_time() const;

  void move_food();

public:
  Game(int rows_, int cols_);
  [[nodiscard]] int get_time();
  void on_key_up(Direction dir);
  void tick();
  void on_new_game();
  void on_reset_game();
  [[nodiscard]] Bitmap render_board();
};
}// namespace snake

#endif