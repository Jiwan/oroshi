#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <tuple>
#include <memory>

#define OROSHI_PROTOCOL_VERSION 0

namespace oroshi
{
namespace common
{
namespace network
{

    class PacketHeader
    {
        public:
        PacketHeader(): size_(0), command_(0), protocolVersion_(OROSHI_PROTOCOL_VERSION)
        {

        }

        ~PacketHeader()
        {

        }

        PacketHeader(PacketHeader& other): size_(other.size_), command_(other.command_), protocolVersion_(other.protocolVersion_)
        {

        }

        PacketHeader& operator=(PacketHeader const& other)
        {
            if (this != &other)
            {
                size_            = other.size_;
                command_         = other.command_;
                protocolVersion_ = other.protocolVersion_;
            }

            return *this;
        }

        PacketHeader(PacketHeader&& other): size_(std::move(other.size_)), command_(std::move(other.command_)), protocolVersion_(std::move(other.protocolVersion_))
        {
            // Note: Actually, std::move isn't necessary for these types (but who know what a PacketHeader will contain later)
        }

        PacketHeader& operator=(PacketHeader&& other)
        {
            if (this != &other)
            {
                size_            = std::move(other.size_);
                command_         = std::move(other.command_);
                protocolVersion_ = std::move(other.protocolVersion_);
            }

            return *this;
        }

        char* data()
        {
            return data_;
        }

        uint16_t size()
        {
            return size_;
        }

        uint16_t command()
        {
            return command_;
        }

        uint16_t protocolVersion()
        {
            return protocolVersion_;
        }

        private:
        union
        {
            char data_[6];

            struct
            {
                uint16_t size_;
                uint16_t command_;
                uint16_t protocolVersion_;
            };
        };
    };

    typedef std::tuple<PacketHeader, std::shared_ptr<char>> Packet;
}
}
}
#endif // PACKET_HPP
