#include <chrono>
#include <numeric>
#include <random>

#include "board.h"

namespace snake {
void Board::reset()
{
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < columns; col++) {
      auto &cell = at(row, col);
      cell.row = row;
      cell.col = col;
      cell.type = CellType::empty;
    }
  }
}

Cell &Board::at(int row, int col) { return cells.at(static_cast<unsigned int>(row * columns + col)); }

[[nodiscard]] const Cell &Board::at(int row, int col) const
{
  return cells.at(static_cast<unsigned int>(row * columns + col));
}


void Board::render(Bitmap &bitmap, int row, int col) const// NOLINT adjacent int parameters
{
  const auto &cell = at(row, col);
  Color color;
  if (cell.type == CellType::snake) {
    color = Color::green;
  } else if (cell.type == CellType::food) {
    color = Color::red;
  } else if (cell.type == CellType::empty) {
    color = Color::light_gray;
  } else {
    color = Color::dark_gray;
  }
  bitmap.set(row, col, { Color::white, color });
}

Board::Board(int rows_, int columns_)
  : rows(rows_), columns(columns_), cells(static_cast<std::vector<Cell>::size_type>(rows * columns))
{
  reset();
}

Bitmap Board::render() const
{
  auto bitmap = Bitmap(rows, columns);
  for (const auto &cell : cells) {
    render(bitmap, cell.row, cell.col);
  }
  return bitmap;
}

void Board::clear()
{
  for (auto &cell : cells) {
    cell.type = CellType::empty;
  }
}

int Board::get_rows() const { return rows; }

int Board::get_columns() const { return columns; }

void Board::set_cell(int row, int col, CellType type)
{
  for (auto& cell : cells)
  {
    if (cell.row == row && cell.col == col) { cell.type = type; }
  }
}

}// namespace snake