#include "login-packet-handler.hpp"

#include <iostream>
#include <functional>

#include <common/utils/log.hpp>

using namespace oroshi::login;
using namespace oroshi::common::utils;
using namespace oroshi::common::network;

#define REGISTER_HANDLER(x, y) handlersMap_[static_cast<uint16_t>(x)] = ThisPacketHandler::Type(std::bind(&LoginPacketHandler::y, this, std::placeholders::_1, std::placeholders::_2));

LoginPacketHandler::LoginPacketHandler()
{
    // !TODO: replace with initialization list when it is supported by mvc++.
    REGISTER_HANDLER(LoginPacketType::ENCRYPTION_REQUEST, handleEncryptionRequest)
    REGISTER_HANDLER(LoginPacketType::USER_LOGIN, handleUserLogin)

}

bool LoginPacketHandler::handle(HANDLER_PARAMS)
{

    std::cout << LogType::LOG_DEBUG << "Received packet : ";
    std::cout << *packet.header();

    if (handlersMap_.find(packet.header()->command()) != handlersMap_.end())
    {
        auto handler = handlersMap_[packet.header()->command()];

        if (!handler(packet, client))
        {
            std::cout << LogType::LOG_ERROR << "Packet wrongly handled" << std::endl;
            std::cout << *packet.header();
        }
    }
    else
    {
        std::cout << LogType::LOG_DEBUG << "No handler found for this packet : ";
        std::cout << packet;
    }

    // TODO: fixed, client->close();

    return true;
}

bool LoginPacketHandler::handleEncryptionRequest(HANDLER_PARAMS) const
{

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

    return true;
}

bool LoginPacketHandler::handleUserLogin(HANDLER_PARAMS)
{
    PacketSource inputStream(packet);

    auto password = inputStream.readFixedSizeString(32);
    auto account  = inputStream.readFixedSizeString(packet.header()->bodySize() - 32);

    std::cout << LogType::LOG_DEBUG << "account: " << account << std::endl;
    std::cout << LogType::LOG_DEBUG << "password: " << password << std::endl;

    return true;
}
