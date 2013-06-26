#ifndef LOG_H
#define LOG_H

#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "../network/packet.hpp"

namespace oroshi
{
namespace common
{
namespace utils
{

    enum class LogType : uint8_t
    {
        NORMAL = 0,
        ERROR,
        HACK,
        DEBUG
    };

    template <class T> T& operator<<(T& t, LogType type)
    {
        t << "[";

        switch (type)
        {
            case LogType::NORMAL:
            t << "NORMAL";
            break;
            case LogType::ERROR:
            t << "ERROR";
            break;
            case LogType::HACK:
            t << "HACK";
            break;
            case LogType::DEBUG:
            t << "DEBUG";
            break;
        }

        auto now   = std::time(nullptr);
        auto local = std::localtime(&now);

        char date[256];
        strftime(date, 256, "%c", local);

        t << "| " << date;
        // TODO: Wait for gcc to implements std::put_time
        // t << "| " << std::put_time(&local, "%c %Z");

        return t << "] ";
    }

    template <class T> T& operator<<(T& t, oroshi::common::network::PacketHeader& packetHeader)
    {
        t << std::hex
          << "command: " << packetHeader.command()
          << "size: "    << packetHeader.size()
          << "version: " << packetHeader.protocolVersion()
          << std::endl;

        return t;
    }

    template <class T> T& operator<<(T& t, oroshi::common::network::Packet& packet)
    {
        auto header  = std::get<0>(packet);
        auto content = std::get<1>(packet);

        // Logs the header.
        t << *header;

        // Logs the content.
        for (uint16_t i = 0; i < header->size(); ++i)
        {
            t << std::hex << content.get()[i];
        }

        t << std::endl;

        return t;
    }

}
}
}
#endif // LOG_H
