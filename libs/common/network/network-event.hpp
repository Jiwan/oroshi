#ifndef NETWORKEVENT_HPP
#define NETWORKEVENT_HPP

#include <memory>

#include "../event.hpp"
#include "network-client.hpp"
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
        std::shared_ptr<NetworkClient> client;
        Packet packet;
    };

}
}
}

#endif