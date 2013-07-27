#ifndef LOGINPACKETHANDLER_H
#define LOGINPACKETHANDLER_H

#include <memory>

#include <common/network/network-client.hpp>
#include <common/network/packet.hpp>
#include <common/network/basic-packet-crypt.hpp>

namespace oroshi
{
namespace login
{

    class LoginPacketHandler
    {
        public:
            bool handle(oroshi::common::network::Packet& packet, 
                std::shared_ptr<oroshi::common::network::NetworkClient<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt>> client);
    };

}
}
#endif // LOGINPACKETHANDLER_H

