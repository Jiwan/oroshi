#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include <functional>
#include <map>
#include <unordered_map>
#include <cstdint>

#include "packet.hpp"
#include "network-client.hpp"

namespace oroshi
{
    namespace common
    {
        namespace network
        {

            /**
            * While the alias templates aren't working, i will use this dummy struct to generate some typedefs.
            */
            template <class PacketHandler, class PacketCrypt, class CoreEngine> struct PacketHandlerTraits
            {
            public:
                typedef std::function<bool (Packet& packet, std::shared_ptr<NetworkClient<PacketHandler, PacketCrypt, CoreEngine>> client)> Type;
                typedef std::map<uint16_t, Type> LightMap;
                typedef std::unordered_map<uint16_t, Type> QuickMap;
            };
        }
    }
}

#endif // PACKETHANDLER_HPP