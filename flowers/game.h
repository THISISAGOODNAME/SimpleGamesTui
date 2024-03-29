#ifndef MINESWEEPER_GAME
#define MINESWEEPER_GAME

#include "board.h"
#include <chrono>

namespace minesweeper {

// Game models the overall game state, including the board, timer, and button interactions.
class Game
{
  enum class GameState { init, playing, ended };

  const int mines_init;

  Board board;

  GameState state = GameState::init;
  int time;
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> start_time{};

  static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> time_now();
  [[nodiscard]] std::chrono::seconds elapsed_time() const;

public:
  Game(int rows_, int cols_, int mines_init_);
  [[nodiscard]] int get_mines() const;
  [[nodiscard]] int get_time();
  void on_mouse_event(int row, int col, bool left_click, bool right_click, bool mouse_up);
  void on_key_up();
  void on_refresh_event();
  void on_new_game();
  void on_reset_game();
  [[nodiscard]] Bitmap render_board() const;
};
}// namespace minesweeper

#endif