#include "login-packet-handler.hpp"

#include <common/utils/log.hpp>

using namespace oroshi::login;
using namespace oroshi::common::utils;

bool LoginPacketHandler::handle(oroshi::common::network::Packet& packet)
{

    std::cout << oroshi::common::utils::LogType::LOG_DEBUG << "Received packet : ";
    std::cout << packet;

    return true;
}
