#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include "game.h"

ftxui::Color map_color(snake::Color color)
{
  if (color == snake::Color::red) { return ftxui::Color::Red; }
  if (color == snake::Color::blue) { return ftxui::Color::Blue; }
  if (color == snake::Color::green) { return ftxui::Color::Green; }
  if (color == snake::Color::dark_blue) { return ftxui::Color::DarkBlue; }
  if (color == snake::Color::dark_red) { return ftxui::Color::DarkRed; }
  if (color == snake::Color::sea_green) { return ftxui::Color::SeaGreen1; }
  if (color == snake::Color::black) { return ftxui::Color::Black; }
  if (color == snake::Color::light_gray) { return ftxui::Color::GrayLight; }
  if (color == snake::Color::dark_gray) { return ftxui::Color::GrayDark; }
  return ftxui::Color::White;
}

ftxui::Canvas bitmap_to_canvas(const snake::Bitmap &bitmap)
{
  auto canvas = ftxui::Canvas(bitmap.get_columns() * 4, bitmap.get_rows() * 4);
  for (int r = 0; r < bitmap.get_rows(); r++) {
    for (int c = 0; c < bitmap.get_columns(); c++) {
      auto pixel = bitmap.get(r, c);
      canvas.DrawText(c * 4, r * 4, std::string{ "  " }, [&pixel](ftxui::Pixel &p) {
        p.foreground_color = map_color(pixel.foreground);
        p.background_color = map_color(pixel.background);
        p.bold = true;
      });
    }
  }
  return canvas;
}

int main()
{
  int rows = 20;
  int cols = 15;
  int mines = 10;
    snake::Game game{ rows, cols, };// NOLINT constant seed parameters for game

  using namespace ftxui;

  auto board_renderer = Renderer([&] { return canvas(bitmap_to_canvas(game.render_board())); });
  auto board_with_mouse = CatchEvent(board_renderer, [&](Event e) { return false; });

  auto new_game_button = Button("New Game", [&] { game.on_new_game(); });

  auto components = CatchEvent(Container::Horizontal({ board_with_mouse, new_game_button }), [&](const Event &e) {
    //if (e.is_character()) { game.on_key_up(); }
    //game.tick();
      if (e == Event::ArrowRight) {
          game.on_key_up(snake::Direction::right);
      } else if (e == Event::ArrowLeft) {
          game.on_key_up(snake::Direction::left);
      } else if (e == Event::ArrowUp) {
          game.on_key_up(snake::Direction::up);
      } else if (e == Event::ArrowDown) {
          game.on_key_up(snake::Direction::down);
      }
    return false;
  });

  auto game_renderer = Renderer(components, [&] {
    return vbox({ center(text("Snake")) | flex,
             separator(),
             hbox({ board_with_mouse->Render(),
               separator(),
               vbox({
                 window(text("Time"), text(std::to_string(game.get_time()))),
                 new_game_button->Render()
               })
             })
            })
            | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1.0s / 10.0);// NOLINT magic numbers
      screen.PostEvent(Event::Custom);
    }
  });
  screen.Loop(game_renderer);
  refresh_ui_continue = false;
  refresh_ui.join();

  return 0;
}