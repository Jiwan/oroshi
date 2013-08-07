#ifndef NETWORKEVENT_HPP
#define NETWORKEVENT_HPP

#include "../event.hpp"
#include "packet.hpp"

namespace oroshi
{
namespace common
{
namespace network
{
    // Forward the children to event.
    template <class Children> class NetworkEvent: Event<Children>
    {
        private:
        Packet packet;
    };

}
}
}

#endif