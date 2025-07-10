#pragma once
#include <cstdint>

namespace trap {
    /* Allocates one Read-Only page (MAP_ANON | MAP_PRIVATE) */
    void *allocate();

    /* Returns true if the page is already bound to a physical frame */
    bool touched(void *addr, uint64_t *raw_entry = nullptr);
}   // namespace trap