#ifndef NETWORKENGINE_HPP
#define NETWORKENGINE_HPP

#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

#include "./network-client.hpp"
#include "../utils/log.hpp"

namespace oroshi
{
namespace common
{
namespace network
{

    /**
     * A PacketHandler must have the following methods:
     *
     * static bool handle(oroshi::common::network::Packet const & packet);
     */

    template <class PacketHandler, int threadCount = 1> class NetworkEngine
    {
        public:
        typedef oroshi::common::network::NetworkClient<PacketHandler> EngineNetworkClient;


        public:
        NetworkEngine() : serverSocket_(nullptr)
        {

        }

        void start()
        {
            start("127.0.0.1", 6666);
        }

        void start(std::string const& ip, unsigned short port)
        {
            auto address = boost::asio::ip::address::from_string(ip);

            start(address, port);
        }

        void start(boost::asio::ip::address const& address, unsigned short port)
        {
            serverSocket_ = std::make_shared<boost::asio::ip::tcp::acceptor>(ioService_, boost::asio::ip::tcp::endpoint(address, port));

            startAccept();

            for (int i = 0; i < threadCount; ++i)
            {
                workerThread_ = std::thread(std::bind(static_cast<std::size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &ioService_));
            }
        }

        void stop()
        {
            workerThread_.join();
        }

        boost::asio::io_service& ioService()
        {
            return ioService_;
        }

        private:
        void startAccept()
        {
            // Start the accept chain.

            auto networkClient = std::make_shared<EngineNetworkClient>(ioService_);

            serverSocket_->async_accept(networkClient->socket(), [this, networkClient] (const boost::system::error_code& e)
            {
                if (!e)
                {
                     std::cout << oroshi::common::utils::LogType::NORMAL << "New client connected" << std::endl;
                     networkClient->start();
                     currentClients_.push_back(networkClient);
                }
                else
                {
                    std::cout << oroshi::common::utils::LogType::NORMAL << e << std::endl;
                }

                startAccept();
            });
        }
         
        private:
        boost::asio::io_service ioService_;
        std::shared_ptr<boost::asio::ip::tcp::acceptor> serverSocket_;
        std::vector<std::shared_ptr<EngineNetworkClient>> currentClients_;
        std::thread workerThread_;
    };

}
}
}

#endif //NETWORKENGINE_HPP
