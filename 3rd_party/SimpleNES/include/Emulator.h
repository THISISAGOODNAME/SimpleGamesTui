#ifndef EMULATOR_H
#define EMULATOR_H
#include <SFML/Graphics.hpp>
#include <chrono>

#include "CPU.h"
#include "PPU.h"
#include "MainBus.h"
#include "PictureBus.h"
#include "Controller.h"

namespace sn
{
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    const int NESVideoWidth = ScanlineVisibleDots;
    const int NESVideoHeight = VisibleScanlines;

    class Emulator
    {
    public:
        Emulator();
        // void run(std::string rom_path);
        // void setVideoWidth(int width);
        // void setVideoHeight(int height);
        // void setVideoScale(float scale);
        // void setKeys(std::vector<sf::Keyboard::Key>& p1, std::vector<sf::Keyboard::Key>& p2);
    private:
        // void DMA(Byte page);

        // MainBus m_bus;
        // PictureBus m_pictureBus;
        // CPU m_cpu;
        // PPU m_ppu;
        // Cartridge m_cartridge;
        // std::unique_ptr<Mapper> m_mapper;

        // TODO remove these, controller is outside of emulator
        // might have to link to a callback still.
        // Controller m_controller1, m_controller2;

        // TODO remove this, shouldn't be a part of the emulator, you'll have to
        // use a callback though on render, so whenever a render is needed, a
        // callback should be called with the virtual screen passed in.
        // sf::RenderWindow m_window;

        // VirtualScreen m_emulatorScreen;
        // float m_screenScale;

        // TimePoint m_cycleTimer;

        // std::chrono::high_resolution_clock::duration m_elapsedTime;
        // std::chrono::nanoseconds m_cpuCycleDuration;
    };
}
#endif // EMULATOR_H
