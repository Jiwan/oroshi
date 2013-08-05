#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

#define OROSHI_PROTOCOL_VERSION 0

namespace io = boost::iostreams;

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

        PacketHeader(PacketHeader&& other): size_(0), command_(0), protocolVersion_(OROSHI_PROTOCOL_VERSION)
        {
            swap(other);
        }

        PacketHeader& operator=(PacketHeader other)
        {
            swap(other);

            return *this;
        }

        void swap(PacketHeader& other)
        {
            std::swap(size_, other.size_);
            std::swap(command_, other.command_);
            std::swap(protocolVersion_, protocolVersion_);
        }

        char* data()
        {
            return data_;
        }

        uint16_t& size()
        {
            return size_;
        }

        uint16_t bodySize()
        {
            return size_ - 6;
        }

        uint16_t& command()
        {
            return command_;
        }

        uint16_t& protocolVersion()
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

    class Packet
    {
        public:
        Packet(std::shared_ptr<PacketHeader> header, std::shared_ptr<char> body): header_(header), body_(body)
        {

        }

        std::shared_ptr<PacketHeader> header()
        {
            return header_;
        }

        std::shared_ptr<char> body()
        {
            return body_;
        }

        private:
        std::shared_ptr<PacketHeader> header_;
        std::shared_ptr<char> body_;
    };

    // Define a new boost "Source" concept for a packet.
    class PacketSource : public io::source
    {
        public:
        PacketSource(Packet& packet) : packet_(packet)
        {

        }

        std::pair<char*, char*> input_sequence()
        {
            auto header = packet_.header();
            auto body   = packet_.body();

            auto sequence = std::make_pair(body.get(), body.get() + header->bodySize());

            return sequence;
        }

        private:
        Packet& packet_;
    };

    // Create an istream from this source.
    typedef io::stream<PacketSource> InputPacketStream;

    class PacketSink
    {
        public:
        PacketSink(uint16_t command) : size_(0), command_(command)
        {

        }

        template<class T> PacketSink& operator<<(T const & t) 
        {
            const char* data = reinterpret_cast<const char*>(&t);

            for (uint16_t i = 0; i < sizeof(T); ++i)
            {
                data_.push_back(data[i]);
            }
                
            size_ += sizeof(T);

            return *this;
        }

        Packet packet()
        {
            auto header = std::shared_ptr<PacketHeader>(new PacketHeader());
            header->command() = command_;
            header->size() = size_ + 6;
            
            auto body = std::shared_ptr<char>(new char[size_]);
            std::memcpy(body.get(), data_.data(), size_);

            return Packet(header, body);
        }

        uint16_t& command()
        {
            return command_;
        }

        private:
        std::vector<char> data_;
        uint16_t size_;
        uint16_t command_;
    };
}
}
}
#endif // PACKET_HPP
