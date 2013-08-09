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
            template <class PacketHandler, class PacketCrypt, class EventHandler = void> class NetworkEvent: EventHandler
            {
            public:
                NetworkEvent(std::shared_ptr<NetworkClient<PacketHandler, PacketCrypt>> client): 
                    client_(client)
                {

                }

            private:
                std::shared_ptr<NetworkClient<PacketHandler, PacketCrypt>> client_;
            };

        }
    }
}

#endif