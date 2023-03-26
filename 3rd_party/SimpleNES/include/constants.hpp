#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
namespace sn {

inline constexpr auto pixel_width  = 256;
inline constexpr auto pixel_height = 240;

inline constexpr auto ScanlineCycleLength = 341;
inline constexpr auto ScanlineEndCycle    = 340;
inline constexpr auto VisibleScanlines    = 240;
inline constexpr auto ScanlineVisibleDots = 256;
inline constexpr auto FrameEndScanline    = 261;
inline constexpr auto AttributeOffset     = 0x3C0;

}  // namespace sn
#endif  // CONSTANTS_HPP
