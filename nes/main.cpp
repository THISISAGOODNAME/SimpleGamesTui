#include <cmath>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/color.hpp"

#include "emulator.hpp"

constexpr auto nes_width  = 256;
constexpr auto nes_height = 240;

unsigned long long color_palette[64] = {
         0x666666,  0x002a88,  0x1412a7,  0x3b00a4,
         0x5c007e,  0x6e0040,  0x6c0600,  0x561d00,
         0x333500,  0x0b4800, 0x005200, 0x004f08,
        0x00404d, 0x000000, 0x000000, 0x000000,
        0xadadad, 0x155fd9, 0x4240ff, 0x7527fe,
        0xa01acc, 0xb71e7b, 0xb53120, 0x994e00,
        0x6b6d00, 0x388700, 0x0c9300, 0x008f32,
        0x007c8d, 0x000000, 0x000000, 0x000000,
        0xfffeff, 0x64b0ff, 0x9290ff, 0xc676ff,
        0xf36aff, 0xfe6ecc, 0xfe8170, 0xea9e22,
        0xbcbe00, 0x88d800, 0x5ce430, 0x45e082,
        0x48cdde, 0x4f4f4f, 0x000000, 0x000000,
        0xfffeff, 0xc0dfff, 0xd3d2ff, 0xe8c8ff,
        0xfbc2ff, 0xfec4ea, 0xfeccc5, 0xf7d8a5,
        0xe4e594, 0xcfef96, 0xbdf4ab, 0xb3f3cc,
        0xb5ebf2, 0xb8b8b8, 0x000000, 0x000000,
};

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Please pass ROM path as first parameter.\n";
        return 1;
    }

    auto const rom_path = argv[1];

    nesterm::Emulator emulator_;
    nesterm::Controller controller1_;
    nesterm::Controller controller2_;
    std::optional<sn::Framebuffer> next_buffer_ = std::nullopt;

    using Clock_t = std::chrono::high_resolution_clock;

    emulator_.register_draw_callback(
            [&,previous_time = Clock_t::now()](sn::Framebuffer const& buf) mutable {
                constexpr auto zero = Clock_t::duration{0};
                constexpr auto period =
                        std::chrono::microseconds{16'639};  // 60.0988139 fps
                auto const to_wait = period - (Clock_t::now() - previous_time);
                if (to_wait > zero)
                    std::this_thread::sleep_for(to_wait);
                previous_time = Clock_t::now();
                next_buffer_  = buf;
            });

    emulator_.set_controller1_read_callback([&] {
        return controller1_.read();
    });
    emulator_.set_controller2_read_callback([&] {
        return controller2_.read();
    });
    emulator_.set_controller_write_callback([&](sn::Byte b) {
        controller1_.strobe(b);
        controller2_.strobe(b);
    });

    emulator_.load_cartridge(rom_path);

    using namespace ftxui;

    auto board_renderer = CatchEvent(Renderer([&] {
        while (!next_buffer_.has_value())
            emulator_.step();  // Might set next_buffer_

        auto c = Canvas(nes_width*2, nes_height*2);

        for (int x = 0; x < nes_width; x++) {
            for (int y = 0; y < nes_height; ++y) {
                sn::Framebuffer const& buf = *next_buffer_;
                auto pixelColor = color_palette[buf[x][y]];
                auto const red = static_cast<uint8_t>(pixelColor >> 16U);
                auto const green = static_cast<uint8_t>(pixelColor >> 8U);
                auto const blue = static_cast<uint8_t>(pixelColor);

                auto pixelColor2 = color_palette[buf[x][y+1]];
                auto const red2 = static_cast<uint8_t>(pixelColor2 >> 16U);
                auto const green2 = static_cast<uint8_t>(pixelColor2 >> 8U);
                auto const blue2 = static_cast<uint8_t>(pixelColor2);

                c.DrawText(x*2, y*2, std::string{  "▀" }, [red, green, blue, red2, green2, blue2](ftxui::Pixel &p) {
                    p.foreground_color = ftxui::Color::RGB(red, green, blue);
                    p.background_color = ftxui::Color::RGB(red2, green2, blue2);
                    p.bold = true;
                });
            }
        }

//        Elements array;
//        int x_length = nes_width;
//        int y_length = nes_height;
//
//        for (int y = 0; y < y_length; y += 2) {
//            Elements line;
//            for (int x = 0; x < x_length; ++x) {
//                sn::Framebuffer const& buf = *next_buffer_;
//                auto pixelColor = color_palette[buf[x][y]];
//                auto const red = static_cast<uint8_t>(pixelColor >> 16U);
//                auto const green = static_cast<uint8_t>(pixelColor >> 8U);
//                auto const blue = static_cast<uint8_t>(pixelColor);
//
//                auto pixelColor2 = color_palette[buf[x][y+1]];
//                auto const red2 = static_cast<uint8_t>(pixelColor2 >> 16U);
//                auto const green2 = static_cast<uint8_t>(pixelColor2 >> 8U);
//                auto const blue2 = static_cast<uint8_t>(pixelColor2);
//
//                line.push_back(text(L"▀") | color(Color::RGB(red, green, blue)) | bgcolor(Color::RGB(red2, green2, blue2)));
//            }
//            array.push_back(hbox(std::move(line)));
//        }

        next_buffer_ = std::nullopt;

        return window(text("SimpleNES"), canvas(c));
//        return window(text("SimpleNES"), vbox(array));
    }), [&](const Event &e) {
        if (e == Event::ArrowUp) {
            controller1_.clear();
            controller1_.press(nesterm::Controller::Button::Up);
        } else if (e == Event::ArrowDown) {
            controller1_.clear();
            controller1_.press(nesterm::Controller::Button::Down);
        } else if (e == Event::ArrowLeft) {
            controller1_.clear();
            controller1_.press(nesterm::Controller::Button::Left);
        } else if (e == Event::ArrowRight) {
            controller1_.clear();
            controller1_.press(nesterm::Controller::Button::Right);
        } else if (e == Event::Backspace) {
            controller1_.clear();
            controller1_.press(nesterm::Controller::Button::Select);
        } else if (e == Event::Return) {
            controller1_.clear();
            controller1_.press(nesterm::Controller::Button::Start);
        } else if (e.is_character()) {
            controller1_.clear();
            if ("z" == e.character()) {
                controller1_.press(nesterm::Controller::Button::A);
            }
            if ("x" == e.character()) {
                controller1_.press(nesterm::Controller::Button::B);
            }
        }
        return false;
    });

    auto screen = ScreenInteractive::FitComponent();
    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1.0s / 60.0);// NOLINT magic numbers
            screen.PostEvent(Event::Custom);
        }
    });
    screen.Loop(board_renderer);
    refresh_ui_continue = false;
    refresh_ui.join();

    return 0;
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
