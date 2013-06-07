#include "./network-engine.hpp"

#include <functional>

#include "../utils/log.hpp"

using namespace boost::asio;
using namespace oroshi::common::network;
using namespace oroshi::common::utils;
using namespace std;

NetworkEngine::NetworkEngine() : serverSocket_(nullptr)
{

}

void NetworkEngine::start()
{
    start("127.0.0.1", 6666);
}

void NetworkEngine::start(std::string const& ip, unsigned short port)
{
    auto address = ip::address::from_string(ip);
    
    start(address, port);
}

void NetworkEngine::start(ip::address const& address, unsigned short port)
{
    serverSocket_ = make_shared<ip::tcp::acceptor>(ioService_, ip::tcp::endpoint(address, port));

    startAccept();

    // Let's start one worker thread.
    // TODO: Add as much workers threads as possible (CPU core count ?)
    workerThread_ = std::thread(std::bind(static_cast<std::size_t (io_service::*)()>(&io_service::run), &ioService_));
}

void NetworkEngine::stop()
{
    workerThread_.join();
}

void NetworkEngine::startAccept()
{
    // Start the accept chain.

    auto networkClient = make_shared<NetworkClient>(ioService_);

    serverSocket_->async_accept(networkClient->socket(), [this, networkClient] (const boost::system::error_code& e)
    {
        if (!e)
        {
             cout << LogType::NORMAL << "New client connected" << endl;
             networkClient->start();
             currentClients_.push_back(networkClient);
        }
        else
        {
            cout << LogType::NORMAL << e << endl;
        }

        startAccept();
    });
}


io_service& NetworkEngine::ioService()
{
    return ioService_;
}
