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
            * bool handle(oroshi::common::network::Packet const & packet);
            */

            template <class PacketHandler, class PacketCrypt, int threadCount = 1> class NetworkEngine
            {
            public:
                typedef oroshi::common::network::NetworkClient<PacketHandler, PacketCrypt> EngineNetworkClient;


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
                    std::cout << oroshi::common::utils::LogType::LOG_NORMAL << "Server starting on interface: " << address.to_string() << " and port: " << port << std::endl;

                    serverSocket_ = std::make_shared<boost::asio::ip::tcp::acceptor>(ioService_, boost::asio::ip::tcp::endpoint(address, port));

                    startAccept();

                    for (int i = 0; i < threadCount; ++i)
                    {
                        workerThread_ = std::thread(std::bind(static_cast<std::size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &ioService_));
                    }
                }

                void removeClient(EngineNetworkClient& client)
                {
                    for (auto it = currentClients_.begin(); it != currentClients_.end(); )
                    {
                        if ((*it).get() == &client)
                        {
                            it = currentClients_.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
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
                            std::cout << oroshi::common::utils::LogType::LOG_NORMAL << "New client connected" << std::endl;

                            // Lambda that will handle the end of the communication between the engine and the client.
                            networkClient->onCloseEvent(std::function<void(EngineNetworkClient& client)>([&](EngineNetworkClient& client) {
                                for (auto it = currentClients_.begin(); it != currentClients_.end(); )
                                {
                                    if ((*it).get() == &client)
                                    {
                                        it = currentClients_.erase(it);
                                    }
                                    else
                                    {
                                        ++it;
                                    }
                                }
                            }));


                            networkClient->start();
                            currentClients_.push_back(networkClient);
                        }
                        else
                        {
                            std::cout << oroshi::common::utils::LogType::LOG_NORMAL << e << std::endl;
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
