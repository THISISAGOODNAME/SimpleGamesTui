#include <cmath>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/color.hpp"

#include "agnes.h"

static void* read_file(const char *filename, size_t *out_len);

constexpr auto nes_width  = AGNES_SCREEN_WIDTH;
constexpr auto nes_height = AGNES_SCREEN_HEIGHT;

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Please pass ROM path as first parameter.\n";
        return 1;
    }

    const char *ines_name = argv[1];

    size_t ines_data_size = 0;
    void* ines_data = read_file(ines_name, &ines_data_size);
    if (ines_data == NULL) {
        fprintf(stderr, "Reading %s failed.\n", ines_name);
        return 1;
    }

    agnes_t *agnes = agnes_make();
    if (agnes == NULL) {
        fprintf(stderr, "Making agnes failed.\n");
        return 1;
    }

    bool ok = agnes_load_ines_data(agnes, ines_data, ines_data_size);
    if (!ok) {
        fprintf(stderr, "Loading %s failed.\n", ines_name);
        return 1;
    }

    agnes_input_t input;

    using namespace ftxui;

    auto board_renderer = CatchEvent(Renderer([&] {
        agnes_set_input(agnes, &input, NULL);

        ok = agnes_next_frame(agnes);
        if (!ok) {
            fprintf(stderr, "Getting next frame failed.\n");
        }

        Elements array;
        int x_length = nes_width;
        int y_length = nes_height;

        for (int y = 0; y < y_length; y += 2) {
            Elements line;
            for (int x = 0; x < x_length; ++x) {
                agnes_color_t c = agnes_get_screen_pixel(agnes, x, y);
                auto const red = c.r;
                auto const green = c.g;
                auto const blue = c.b;

                agnes_color_t c2 = agnes_get_screen_pixel(agnes, x, y+1);
                auto const red2 = c2.r;
                auto const green2 = c2.g;
                auto const blue2 = c2.b;

                line.push_back(text(L"▀") | color(Color::RGB(red, green, blue)) | bgcolor(Color::RGB(red2, green2, blue2)));
            }
            array.push_back(hbox(std::move(line)));
        }

        return window(text("SimpleNES"), vbox(array));
    }), [&](const Event &e) {
        memset(&input, 0, sizeof(agnes_input_t));
        if (e == Event::ArrowUp)       input.up     = true;
        if (e == Event::ArrowDown)     input.down   = true;
        if (e == Event::ArrowLeft)     input.left   = true;
        if (e == Event::ArrowRight)    input.right  = true;
        if (e == Event::Backspace)     input.select = true;
        if (e == Event::Return)        input.start  = true;
        if (e.is_character()) {
            if ("z" == e.character())  input.a = true;
            if ("x" == e.character())  input.b = true;
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

static void* read_file(const char *filename, size_t *out_len) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    long pos = ftell(fp);
    if (pos < 0) {
        fclose(fp);
        return NULL;
    }
    size_t file_size = pos;
    rewind(fp);
    unsigned char *file_contents = (unsigned char *)malloc(file_size);
    if (!file_contents) {
        fclose(fp);
        return NULL;
    }
    if (fread(file_contents, file_size, 1, fp) < 1) {
        if (ferror(fp)) {
            fclose(fp);
            free(file_contents);
            return NULL;
        }
    }
    fclose(fp);
    *out_len = file_size;
    return file_contents;
}
