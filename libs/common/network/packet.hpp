#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <tuple>
#include <memory>

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

    typedef std::tuple<std::shared_ptr<PacketHeader>, std::shared_ptr<char>> Packet;
	
	// Define a new boost "Source" concept for a packet.
	class PacketSource : public io::source
	{
		public:
		PacketSource(Packet& packet) : packet_(packet)
		{

		}

		std::pair<char*, char*> input_sequence()
		{
			auto header = std::get<0>(packet_);
			auto body   = std::get<1>(packet_);

			auto sequence = std::make_pair(body.get(), body.get() + header->bodySize());

			return sequence;
		}

		private:
		Packet& packet_;
	};

	// Create a istream from this source.
	typedef io::stream<PacketSource> InputPacketStream;

	
	/*
	class PacketSink : public io::sink 
	{
		public:

		std::streamsize write(const char* s, std::streamsize n) 
		{

		}
	};
	*/
	

}
}
}
#endif // PACKET_HPP
