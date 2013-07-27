#include "login-packet-handler.hpp"

#include <common/utils/log.hpp>

using namespace oroshi::login;
using namespace oroshi::common::utils;
using namespace oroshi::common::network;

bool LoginPacketHandler::handle(Packet& packet, 
                std::shared_ptr<NetworkClient<LoginPacketHandler, BasicPacketCrypt>> client)
{

    std::cout << oroshi::common::utils::LogType::LOG_DEBUG << "Received packet : ";
    std::cout << packet;

    client->close();

    return true;
}
