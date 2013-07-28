#ifndef LOG_H
#define LOG_H

#ifdef _MSC_VER 
#if (_MSC_VER >= 1700)
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#endif

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
        LOG_NORMAL = 0,
        LOG_ERROR,
        LOG_HACK,
        LOG_DEBUG
    };

    template <class T> T& operator<<(T& t, LogType type)
    {
        t << "[";

        switch (type)
        {
            case LogType::LOG_NORMAL:
            t << "NORMAL";
            break;
            case LogType::LOG_ERROR:
            t << "ERROR";
            break;
            case LogType::LOG_HACK:
            t << "HACK";
            break;
            case LogType::LOG_DEBUG:
            t << "DEBUG";
            break;
        }

        auto now   = std::time(nullptr);
        auto local = std::localtime(&now);

        char date[256];
        strftime(date, 256, "%c", local);

        t << " | " << date;
        // TODO: Wait for gcc to implements std::put_time
        // t << "| " << std::put_time(&local, "%c %Z");

        return t << "] ";
    }

    template <class T> T& operator<<(T& t, oroshi::common::network::PacketHeader& packetHeader)
    {
        t << std::hex
          << "command: " << packetHeader.command()
          << " size: "    << packetHeader.size()
          << " version: " << packetHeader.protocolVersion()
          << std::endl;

        return t;
    }

    template <class T> T& operator<<(T& t, oroshi::common::network::Packet& packet)
    {
        auto header  = packet.header();
        auto body    = packet.body();

        // Logs the header.
        t << *header;

        // Logs the content.
        for (uint16_t i = 0; i < header->bodySize(); ++i)
        {
            t << std::hex << static_cast<uint32_t>(static_cast<uint8_t>(body.get()[i]));
        }

        t << std::endl;

        return t;
    }

}
}
}
#endif // LOG_H
