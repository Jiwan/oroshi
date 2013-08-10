#ifndef LOGINEVENT_HPP
#define LOGINEVENT_HPP

#include <string>

#include <common/network/basic-packet-crypt.hpp>
#include <common/network/network-client.hpp>
#include <common/network/network-event.hpp>

// Note: I am not sure if a forward declaration of LoginPacketHandler is necessary (a good old include could be enough).

namespace oroshi
{
    namespace login
    {
        class LoginPacketHandler;
        template <int threadCount> class LoginCoreEngine;
        typedef LoginCoreEngine<2> CurrentLoginCoreEngine;

        typedef oroshi::common::network::NetworkEvent<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt, CurrentLoginCoreEngine> LoginNetworkEvent;
        typedef oroshi::common::network::NetworkClient<LoginPacketHandler, oroshi::common::network::BasicPacketCrypt, CurrentLoginCoreEngine> LoginNetworkClient;

        class UserLoginEvent: LoginNetworkEvent
        {
        public:
            UserLoginEvent(std::shared_ptr<LoginNetworkClient> client, std::string& account, std::string& password):
                LoginNetworkEvent(client),
                account_(account),
                password_(password)
            {

            }

            const std::string& account() const
            {
                return account_;
            }

            const std::string& password() const
            {
                return password_;
            }

        private:
            std::string account_;
            std::string password_;
        };
    }
}

#endif // LOGINEVENT_HPP