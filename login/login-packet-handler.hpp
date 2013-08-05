#ifndef LOGINPACKETHANDLER_HPP
#define LOGINPACKETHANDLER_HPP

#include <cstdint>
#include <memory>

#include <common/network/basic-packet-crypt.hpp>
#include <common/network/network-client.hpp>
#include <common/network/packet.hpp>
#include <common/network/packet-handler.hpp>

namespace oroshi
{
namespace login
{

    #define HANDLER_PARAMS oroshi::common::network::Packet& packet, std::shared_ptr<oroshi::common::network::NetworkClient<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt>> client

    enum class LoginPacketType : std::uint16_t
    {
        ENCRYPTION_REQUEST = 0x703,
    };

    class LoginPacketHandler
    {
        private:
        typedef oroshi::common::network::PacketHandlerTraits<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt> ThisPacketHandler;
        
        public:
        LoginPacketHandler();
        bool handle(HANDLER_PARAMS);

        private:
        bool handleEncryptionRequest(HANDLER_PARAMS);

        private:
            ThisPacketHandler::QuickMap handlersMap_;
    };

}
}
#endif // LOGINPACKETHANDLER_HPP

