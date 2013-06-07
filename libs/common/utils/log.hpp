#ifndef LOG_H
#define LOG_H

#include <cstdint>
#include <ctime>
#include <iomanip>

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

    // TODO: log a packet

}
}
}
#endif // LOG_H
