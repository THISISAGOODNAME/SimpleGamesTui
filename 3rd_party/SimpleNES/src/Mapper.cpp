#include "Mapper.h"

#include <memory>

#include "MapperCNROM.h"
#include "MapperNROM.h"
#include "MapperSxROM.h"
#include "MapperUxROM.h"

namespace sn {
NameTableMirroring Mapper::getNameTableMirroring()
{
    return static_cast<NameTableMirroring>(m_cartridge.getNameTableMirroring());
}

std::unique_ptr<Mapper> Mapper::createMapper(
    Mapper::Type mapper_t,
    sn::Cartridge& cart,
    std::function<void(void)> mirroring_cb)
{
    switch (mapper_t) {
        case NROM: return std::make_unique<MapperNROM>(cart);
        case SxROM: return std::make_unique<MapperSxROM>(cart, mirroring_cb);
        case UxROM: return std::make_unique<MapperUxROM>(cart);
        case CNROM: return std::make_unique<MapperCNROM>(cart);
        default: return nullptr;
    }
}

}  // namespace sn
