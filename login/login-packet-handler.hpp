#ifndef LOGINPACKETHANDLER_H
#define LOGINPACKETHANDLER_H

#include <common/network/packet.hpp>
namespace oroshi
{
namespace login
{

    class LoginPacketHandler
    {
        public:
        bool handle(oroshi::common::network::Packet& packet);
    };

}
}
#endif // LOGINPACKETHANDLER_H

