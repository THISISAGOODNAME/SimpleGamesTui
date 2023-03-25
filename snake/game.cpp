#include "game.h"
#include <iostream>
#include <random>

namespace snake {
std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> Game::time_now()
{
  return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
}

std::chrono::seconds Game::elapsed_time() const
{
  auto now = time_now();
  return std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
}

Game::Game(int rows_, int cols_)
  : board(rows_, cols_)
  , time(0)
  , start_time(time_now())
{}

int Game::get_time()
{
  if (snake_alive)
    time = static_cast<int>(elapsed_time().count());
  return time;
}

void Game::tick()
{
  if (snake_alive) {
    if (directionQueue.size() > 1) directionQueue.erase(directionQueue.begin());
    int nextXPosition = snakeSegments[0].x;
    int nextYPosition = snakeSegments[0].y;

    if (directionQueue[0] == Direction::right) {
      nextXPosition = nextXPosition + 1;
      if (nextXPosition >= board.get_columns()) {
        nextXPosition = 0;
      }
    } else if (directionQueue[0] == Direction::left) {
     nextXPosition = nextXPosition - 1;
     if (nextXPosition < 0) {
       nextXPosition = board.get_columns() - 1;
     }
    } else if (directionQueue[0] == Direction::down) {
      nextYPosition = nextYPosition + 1;
      if (nextYPosition >= board.get_rows()) {
        nextYPosition = 0;
      }
    } else if (directionQueue[0] == Direction::up) {
      nextYPosition = nextYPosition - 1;
      if (nextYPosition < 0) {
        nextYPosition = board.get_rows() - 1;
      }
    }

    bool canMove = true;
    for (auto& seg : snakeSegments) {
      if (nextXPosition == seg.x && nextYPosition == seg.y) {
        canMove = false;
        break;
      }
    }

    if (canMove) {
      snakeSegments.insert(snakeSegments.begin(), {nextXPosition, nextYPosition,});

      if (snakeSegments[0].x == foodPosition.x && snakeSegments[0].y == foodPosition.y) {
        move_food();
      } else {
        snakeSegments.pop_back();
      }
    } else {
      snake_alive = false;
    }

    board.set_cell(foodPosition.y, foodPosition.x, CellType::food);

    for (auto& snakeSeg : snakeSegments) {
      board.set_cell(snakeSeg.y, snakeSeg.x, CellType::snake);
    }
  } else {
    for (auto& snakeSeg : snakeSegments) {
      board.set_cell(snakeSeg.y, snakeSeg.x, CellType::dead);
    }
  }
}

void Game::move_food() {
  std::vector<Point> possibleFoodPositions{};

  for (int foodY = 0; foodY < board.get_rows(); foodY++) {
    for (int foodX = 0; foodX < board.get_columns(); foodX++) {
      bool possible = true;

      for (auto& snakeSegment : snakeSegments) {
        if (snakeSegment.x == foodX && snakeSegment.y == foodY) {
          possible = false;
          break;
        }
      }

      if (possible) {
        possibleFoodPositions.push_back({foodX, foodY});
      }
    }
  }

  if ((int)possibleFoodPositions.size() > 0) {
    auto now = std::chrono::system_clock::now();
    auto second_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    std::mt19937 mt{ static_cast<unsigned int>(second_since_epoch) };
    std::uniform_int_distribution dist{ 0, (int)possibleFoodPositions.size() - 1 };// random values over closed (inclusive) range
    int nextRandom = dist(mt);
    foodPosition = possibleFoodPositions[nextRandom];
  }
}

void Game::on_new_game()
{
  on_reset_game();
}

void Game::on_reset_game()
{
  time = 0;
  start_time = time_now();
  snakeSegments = {
    { 3, 1 },
    { 2, 1 },
    { 1, 1 },
  };
  directionQueue = { Direction::right };
  snake_alive = true;

  board.reset();

  move_food();
}

Bitmap Game::render_board(){
  board.clear();
  tick();
  return board.render();
}
void Game::on_key_up(Direction dir)
{
  int lastdirQueueIdx = (int)directionQueue.size() - 1;
  if (dir == Direction::right && directionQueue[lastdirQueueIdx] != Direction::left) { directionQueue.push_back(dir); }
  else if (dir == Direction::left && directionQueue[lastdirQueueIdx] != Direction::right) { directionQueue.push_back(dir); }
  else if (dir == Direction::up && directionQueue[lastdirQueueIdx] != Direction::down) { directionQueue.push_back(dir); }
  else if (dir == Direction::down && directionQueue[lastdirQueueIdx] != Direction::up) { directionQueue.push_back(dir); }
}

}// namespace snake