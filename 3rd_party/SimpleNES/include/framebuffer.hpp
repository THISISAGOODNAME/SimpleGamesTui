#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <array>

#include "Cartridge.h"
#include "constants.hpp"

namespace sn {

/// 2D array of Bytes, each cooresponding to a pixel's index into the palette.
using Framebuffer =
    std::array<std::array<Byte, VisibleScanlines>, ScanlineVisibleDots>;

}  // namespace sn
#endif  // FRAMEBUFFER_HPP
