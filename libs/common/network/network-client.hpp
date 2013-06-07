#ifndef NETWORKCLIENT_HPP
#define NETWORKCLIENT_HPP

#include <boost/asio.hpp>
#include <memory>
#include <tuple>

#include "./packet.hpp"
#include "../utils/log.hpp"

namespace oroshi
{
namespace common
{
namespace network
{

    template <class PacketHandler> class NetworkClient
    {
        public:
        NetworkClient(boost::asio::io_service& ioService): ioService_(ioService), socket_(ioService)
        {

        }

        void start()
        {
            // Avoid the naggle algorithm on packets.
            boost::asio::ip::tcp::no_delay optionNoDelay(true);
            socket_.set_option(optionNoDelay);

            socket_.async_receive(boost::asio::buffer(currentHeader_.data(), 6), bind(&NetworkClient::handlePacketHeader, this, std::placeholders::_1, std::placeholders::_2));
        }

        void close()
        {
            ioService_.post(bind(&NetworkClient::do_close, this));
        }

        boost::asio::ip::tcp::socket& socket()
        {
            return socket_;
        }

        private:
        void handlePacketHeader(const boost::system::error_code& error, size_t bytesTransfered)
        {
            if (error)
            {
                std::cout << oroshi::common::utils::LogType::ERROR << "Error while receiving a packet header: " << error << std::endl;
                do_close();

                return;
            }

            if (bytesTransfered < 6)
            {
                std::cout << oroshi::common::utils::LogType::ERROR << "Invalid packet received: header size < 6" << std::endl;
                do_close();

                return;
            }

            // Allocate memory for the incoming packet body.
            std::shared_ptr<char> packetBody(new char[currentHeader_.size()], std::default_delete<char[]>());

            // Let's receive the packet body according to the header.
            socket_.async_receive(boost::asio::buffer(packetBody.get(), currentHeader_.size()), std::bind(&NetworkClient<PacketHandler>::handlePacketBody,
                                                                                              this,
                                                                                              std::placeholders::_1,
                                                                                              std::placeholders::_2,
                                                                                              packetBody
                                                                                              ));
        }

        void handlePacketBody(const boost::system::error_code&  error, size_t bytesTransfered, std::shared_ptr<char> packetBody)
        {
            if (error)
            {
                std::cout << oroshi::common::utils::LogType::ERROR << "Error while receiving a packet header: " << error << std::endl;
                do_close();

                return;
            }

            if (bytesTransfered < currentHeader_.size())
            {
                std::cout << oroshi::common::utils::LogType::ERROR << "Invalid packet received: size of packet < size from header" << std::endl;
                do_close();

                return;
            }

            auto status = PacketHandler::handle(std::make_tuple(std::move(currentHeader_), packetBody));

            socket_.async_receive(boost::asio::buffer(currentHeader_.data(), 6),
                                  std::bind(&NetworkClient<PacketHandler>::handlePacketHeader,
                                  this,
                                  std::placeholders::_1, std::placeholders::_2));
        }

        void do_close()
        {
            std::cout << oroshi::common::utils::LogType::DEBUG << "Closing socket..." << std::endl;

            socket_.close();

            std::cout << oroshi::common::utils::LogType::NORMAL << "Client socket closed" << std::endl;
        }

        private:
        boost::asio::ip::tcp::socket socket_;
        boost::asio::io_service& ioService_;
        PacketHeader currentHeader_;
    };

}
}
}
#endif // NETWORKCLIENT_HPP
