#ifndef NETWORKCLIENT_HPP
#define NETWORKCLIENT_HPP

#include <boost/asio.hpp>
#include <memory>

#include "./packet.hpp"

namespace oroshi
{
namespace common
{
namespace network
{

    class NetworkClient
    {
        public:
        NetworkClient(boost::asio::io_service& ioService);

        void start();
        void sendPacket();
        void close();

        boost::asio::ip::tcp::socket& socket();

        private:
        void handlePacketHeader(const boost::system::error_code& error, size_t bytesTransfered);
        void handlePacketBody(const boost::system::error_code& error, size_t bytesTransfered, std::shared_ptr<char> packetBody);
        void do_close();

        private:
        boost::asio::ip::tcp::socket socket_;
        boost::asio::io_service& ioService_;
        PacketHeader currentHeader_;
    };

}
}
}
#endif // NETWORKCLIENT_HPP
