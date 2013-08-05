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
    /**
     * Here is how the receive loop roughly looks like.
     * Any error during each step will call doClose and stop the loop.
     * 
     *                                                  size > 0
     * start ==> startReceive ==> handlePacketHeader =========> handlePacketBody =========> PacketCrypt::decrypt ========> PacketHandler::handle
     *             ^                           | size == 0                                                                                   |
     *             |===================== PacketCrypt::decrypt                                                                               |
     *             |                                                                                                                         |
     *             |=========================================================================================================================|
     */

    template <class PacketHandler, class PacketCrypt> class NetworkClient : 
        private PacketHandler, 
        private PacketCrypt, 
        public std::enable_shared_from_this<NetworkClient<PacketHandler, PacketCrypt>>
    {
        using PacketHandler::handle;
        using PacketCrypt::decrypt;
    
    public:
        typedef NetworkClient<PacketHandler, PacketCrypt> ThisType; 

    public:
        NetworkClient(boost::asio::io_service& ioService): ioService_(ioService), socket_(ioService)
        {

        }

        void start()
        {
            // Avoid the naggle algorithm on packets.
            boost::asio::ip::tcp::no_delay optionNoDelay(true);
            socket_.set_option(optionNoDelay);

            startReceive();
        }

        void sendPacket(Packet packet)
        {
             ioService_.post(bind(&NetworkClient::doSend, this, packet));
        }

        void close()
        {
            ioService_.post(bind(&NetworkClient::doClose, this));
        }

        boost::asio::ip::tcp::socket& socket()
        {
            return socket_;
        }

        void onCloseEvent(std::function<void(ThisType& client)>& f)
        {
            onCloseEvent_ = f;
        }

    private:
        void startReceive()
        {
            auto packetHeader = std::make_shared<PacketHeader>();

            socket_.async_receive(boost::asio::buffer(packetHeader->data(), 6), bind(&NetworkClient::handlePacketHeader,
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                packetHeader));
        }

        void handlePacketHeader(const boost::system::error_code& error, size_t bytesTransfered, std::shared_ptr<PacketHeader> packetHeader)
        {
            if (error)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Error while receiving a packet header: " << error << std::endl;
                doClose();

                return;
            }

            if (bytesTransfered < 6)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Invalid packet received: header size < 6" << std::endl;
                doClose();

                return;
            }

            // Let's check if the packet have a body.
            if (packetHeader->bodySize())
            {

                // Allocate memory for the incoming packet body.
                std::shared_ptr<char> packetBody(new char[packetHeader->size()], std::default_delete<char[]>());

                // Let's receive the packet body according to the header.
                socket_.async_receive(boost::asio::buffer(packetBody.get(), packetHeader->size()), std::bind(&NetworkClient<PacketHandler, 
                    PacketCrypt>::handlePacketBody,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    packetBody,
                    packetHeader
                    ));
            }
            else
            {
                Packet packet(packetHeader, std::shared_ptr<char>(nullptr));

                if (handlePacket(packet))
                {
                    startReceive();
                }    
            }
        }

        void handlePacketBody(const boost::system::error_code&  error, size_t bytesTransfered, std::shared_ptr<char> packetBody, std::shared_ptr<PacketHeader> currentHeader)
        {
            if (error)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Error while receiving a packet header: " << error << std::endl;
                doClose();

                return;
            }

            if (bytesTransfered < currentHeader->size() - 6)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Invalid packet received: size of packet body < size from header" << std::endl;
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Announced: " << currentHeader->bodySize() << ", real: " << bytesTransfered << std::endl;
                doClose();

                return;
            }

            Packet packet(currentHeader, packetBody);

            if (handlePacket(packet))
            {
                startReceive();
            }            
        }

        bool handlePacket(Packet& packet)
        {
            decrypt(packet);

            bool status = handle(packet, shared_from_this());

            if (!status)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Error while handling packet" << std::endl;
                doClose();
            }

            return status;
        }

        void doSend(Packet packet)
        {
            auto header = boost::asio::buffer(packet.header()->data(), 6);
            auto body   = boost::asio::buffer(packet.body().get(), packet.header()->bodySize());

            std::vector<boost::asio::const_buffer> total;

            total.push_back(header);
            total.push_back(body);

            socket_.async_send(total, bind(&NetworkClient::handleSendedPacket, 
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                packet
                ));
        }

        void handleSendedPacket(const boost::system::error_code& error, std::size_t /*length*/, Packet pack)
        {
            if (error)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Error while sending a packet" << error << std::endl;
                doClose();

                return;
            }
        }

        void doClose()
        {
            std::cout << oroshi::common::utils::LogType::LOG_DEBUG << "Closing socket..." << std::endl;

            socket_.close();

            if (onCloseEvent_)
            {
                // Let's notice the engine that we closed the communication with the client.
                onCloseEvent_(*this);
            }

            std::cout << oroshi::common::utils::LogType::LOG_NORMAL << "Client socket closed" << std::endl;
        }

        private:
        boost::asio::ip::tcp::socket socket_;
        boost::asio::io_service& ioService_;
        std::function<void(ThisType& client)> onCloseEvent_;
    };

}
}
}
#endif // NETWORKCLIENT_HPP