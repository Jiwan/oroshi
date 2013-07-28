#include "login-packet-handler.hpp"

#include <common/utils/log.hpp>
#include <iostream>

using namespace oroshi::login;
using namespace oroshi::common::utils;
using namespace oroshi::common::network;

bool LoginPacketHandler::handle(Packet& packet, 
                std::shared_ptr<NetworkClient<LoginPacketHandler, BasicPacketCrypt>> client)
{

    std::cout << oroshi::common::utils::LogType::LOG_DEBUG << "Received packet : ";
    std::cout << packet;

    PacketSink outputStream(0x7ff);

    outputStream << static_cast<uint16_t>(0xaf02);
    outputStream << static_cast<uint16_t>(0xbd46);
    outputStream << static_cast<uint16_t>(0x9);
    outputStream << static_cast<uint16_t>(0x12);
    outputStream << static_cast<uint8_t>(0x0);
    outputStream << static_cast<uint32_t>(0xcdcdcdcd);
    outputStream << static_cast<uint32_t>(0xcdcdcdcd);
    outputStream << static_cast<uint32_t>(0xcdcdcdcd);
    outputStream << static_cast<uint32_t>(0xcdcdcdcd);
    outputStream << static_cast<uint16_t>(0xcdcd);
    outputStream << static_cast<uint8_t>(0xd3);

    auto cryptPacket = outputStream.packet();

    client->sendPacket(cryptPacket);


    // TODO: fixed, client->close();

    return true;
}
