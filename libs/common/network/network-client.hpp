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
	 * Any error during each step will call do_close and stop the loop.
	 * 
	 *												  size > 0
	 * start ==> startReceive ==> handlePacketHeader =========> handlePacketBody =========> PacketCrypt::decrypt ========> PacketHandler::handle
	 *			 ^                           | size == 0                                                                                   |
	 *			 |===================== PacketCrypt::decrypt                                                                               |
	 *           |                                                                                                                         |
	 *           |=========================================================================================================================|
	 */

    template <class PacketHandler, class PacketCrypt> class NetworkClient : private PacketHandler, private PacketCrypt
    {
        using PacketHandler::handle;
		using PacketCrypt::decrypt;

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

        void close()
        {
            ioService_.post(bind(&NetworkClient::do_close, this));
        }

        boost::asio::ip::tcp::socket& socket()
        {
            return socket_;
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
                do_close();

                return;
            }

            if (bytesTransfered < 6)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Invalid packet received: header size < 6" << std::endl;
                do_close();

                return;
            }

			// Let's check if the packet have a body.
			if (!packetHeader->size())
			{
				
			}

            // Allocate memory for the incoming packet body.
            std::shared_ptr<char> packetBody(new char[packetHeader->size()], std::default_delete<char[]>());

            // Let's receive the packet body according to the header.
            socket_.async_receive(boost::asio::buffer(packetBody.get(), packetHeader->size()), std::bind(&NetworkClient<PacketHandler>::handlePacketBody,
                                                                                              this,
                                                                                              std::placeholders::_1,
                                                                                              std::placeholders::_2,
                                                                                              packetBody,
                                                                                              packetHeader
                                                                                              ));
        }

        void handlePacketBody(const boost::system::error_code&  error, size_t bytesTransfered, std::shared_ptr<char> packetHeader, std::shared_ptr<PacketHeader> currentHeader)
        {
            if (error)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Error while receiving a packet header: " << error << std::endl;
                do_close();

                return;
            }

            if (bytesTransfered < currentHeader->size())
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Invalid packet received: size of packet < size from header" << std::endl;
                do_close();

                return;
            }

            auto packet = std::make_tuple(currentHeader, packetHeader);
		
			if (handlePacket(packet))
			{
				startReceive();
			}			
        }

		bool handlePacket(Packet& packet)
		{
			// TODO: decrypt.

			decrypt(packet);
			 
			bool status = handle(packet);

			if (!status)
            {
                std::cout << oroshi::common::utils::LogType::LOG_ERROR << "Error while handling packet" << std::endl;
                do_close();
            }

			return status;
		}

        void do_close()
        {
            std::cout << oroshi::common::utils::LogType::LOG_DEBUG << "Closing socket..." << std::endl;

            socket_.close();

            std::cout << oroshi::common::utils::LogType::LOG_NORMAL << "Client socket closed" << std::endl;
        }

        private:
        boost::asio::ip::tcp::socket socket_;
        boost::asio::io_service& ioService_;
    };

}
}
}
#endif // NETWORKCLIENT_HPP
