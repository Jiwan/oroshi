#ifndef NETWORKENGINE_HPP
#define NETWORKENGINE_HPP

#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

#include "./network-client.hpp"

namespace oroshi
{
namespace common
{
namespace network
{

    class NetworkEngine 
    {
        public:
        NetworkEngine();

        void start();
        void start(std::string const& ip, unsigned short port);
        void start(boost::asio::ip::address const& address, unsigned short port);

        void stop();
        
        boost::asio::io_service& ioService();     
        
        private:
        void startAccept();
         
        private:
        boost::asio::io_service ioService_;
        // TODO: Maybe use a unique_ptr for this one ?
        std::shared_ptr<boost::asio::ip::tcp::acceptor> serverSocket_;
        std::vector<std::shared_ptr<NetworkClient>> currentClients_;
        std::thread workerThread_;
    };

}
}
}

#endif //NETWORKENGINE_HPP
