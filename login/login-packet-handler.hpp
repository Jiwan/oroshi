#ifndef LOGINPACKETHANDLER_HPP
#define LOGINPACKETHANDLER_HPP

#include <cstdint>
#include <memory>

#include <common/network/basic-packet-crypt.hpp>
#include <common/network/network-client.hpp>
#include <common/network/packet.hpp>
#include <common/network/packet-handler.hpp>

#include "login-core-engine.hpp"

namespace oroshi
{
    namespace login
    {

        #define HANDLER_PARAMS oroshi::common::network::Packet& packet, std::shared_ptr<oroshi::common::network::NetworkClient<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt, CurrentLoginCoreEngine>> client

        enum class LoginPacketType : std::uint16_t
        {
            ENCRYPTION_REQUEST = 0x703,
            USER_LOGIN = 0x708,
            GET_SERVER_LIST = 0x704,
            GET_SERVER_IP = 0x70a,
        };

        class LoginPacketHandler
        {
        private:
            typedef oroshi::common::network::PacketHandlerTraits<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt, CurrentLoginCoreEngine> ThisPacketHandler;

        public:
            LoginPacketHandler(CurrentLoginCoreEngine& coreEngine);

            bool handle(HANDLER_PARAMS);

        private:
            // Well, in the login server case i choose to dispatch all events to their handlers statically.
            template <class EventType> void dispatchEvent(EventType& event)
            {
                coreEngine_.handle(event);
            }

            bool handleEncryptionRequest(HANDLER_PARAMS) const;
            bool handleUserLogin(HANDLER_PARAMS);

        private:
            ThisPacketHandler::QuickMap handlersMap_;
            CurrentLoginCoreEngine& coreEngine_;
        };

    }
}
#endif // LOGINPACKETHANDLER_HPP

