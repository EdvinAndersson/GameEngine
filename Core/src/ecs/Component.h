#pragma once
#include <bitset>
#include <cstdint>

namespace CW {

    using ComponentType = uint8_t;

    const ComponentType MAX_COMPONENTS = 32;

    using Signature = std::bitset<MAX_COMPONENTS>;

}