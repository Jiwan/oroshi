#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

namespace oroshi
{
    namespace common
    {
        // This class use the Curiously recurring template pattern in order to avoid the cost of a virtual table.
        template <class Children> class Event
        {
            uint8_t type()
            {
                return reinterpret_cast<Children *>(*this)->type();
            }
        };
    }
}
#endif