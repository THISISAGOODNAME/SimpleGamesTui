#pragma once

#include "bitmap.h"
#include <array>
#include <vector>
#include <functional>

namespace snake {

enum class CellType { empty, snake, dead, food };

struct Cell
{
  int row;
  int col;
  CellType type;
};

class Board
{
  const int rows;
  const int columns;

  std::vector<Cell> cells;


  Cell &at(int row, int col);
  [[nodiscard]] const Cell &at(int row, int col) const;
  void render(Bitmap &bitmap, int row, int col) const;

public:
  explicit Board(int rows_, int columns_);
  [[nodiscard]] Bitmap render() const;
  void clear();
  void set_cell(int row, int col, CellType type);
  void reset();
  [[nodiscard]] int get_rows() const;
  [[nodiscard]] int get_columns() const;
};
}// namespace snake
