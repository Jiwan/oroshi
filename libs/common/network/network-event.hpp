#ifndef NETWORKEVENT_HPP
#define NETWORKEVENT_HPP

#include <memory>
#include <tuple>

#include "../event.hpp"
#include "network-client.hpp"
#include "packet.hpp"

namespace oroshi
{
    namespace common
    {
        namespace network
        {
            // For std::tuple<>, see : http://stackoverflow.com/questions/16666871/is-empty-struct-defined-by-c-standard

            template <class PacketHandler, class PacketCrypt, class EventHandler = void> class NetworkEvent:
            std::conditional<std::is_same<EventHandler, void>::value, std::tuple<>, Event<EventHandler>>::type
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