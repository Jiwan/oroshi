#include "./network-client.hpp"

#include <array>
#include <functional>

#include "../utils/log.hpp"

using namespace oroshi::common::network;
using namespace oroshi::common::utils;
using namespace boost::asio;
using namespace std;

NetworkClient::NetworkClient(io_service& ioService): ioService_(ioService), socket_(ioService)
{

}

void NetworkClient::start()
{
    // Avoid the naggle algorithm on packets.
    ip::tcp::no_delay optionNoDelay(true);
    socket_.set_option(optionNoDelay);

    socket_.async_receive(buffer(currentHeader_.data(), 6), bind(&NetworkClient::handlePacketHeader, this, std::placeholders::_1, std::placeholders::_2));
}

void NetworkClient::close()
{
    ioService_.post(bind(&NetworkClient::do_close, this));
}

void NetworkClient::handlePacketHeader(const boost::system::error_code& error, size_t bytesTransfered)
{
    if (error)
    {
        cout << LogType::ERROR << "Error while receiving a packet header: " << error << endl;
        do_close();

        return;
    }

    if (bytesTransfered < 6)
    {
        cout << LogType::ERROR << "Invalid packet received: header size < 6" << endl;
        do_close();

        return;
    }

    // Allocate memory for the incoming packet body.
    shared_ptr<char> packetBody(new char[currentHeader_.size()], default_delete<char[]>());

    // Let's receive the packet body according to the header.
    socket_.async_receive(buffer(packetBody.get(), currentHeader_.size()), bind(&NetworkClient::handlePacketBody,
                                                                                      this,
                                                                                      std::placeholders::_1,
                                                                                      std::placeholders::_2,
                                                                                      packetBody
                                                                                      ));
}

void NetworkClient::handlePacketBody(const boost::system::error_code&  error, size_t bytesTransfered, shared_ptr<char> packetBody)
{
    if (error)
    {
        cout << LogType::ERROR << "Error while receiving a packet header: " << error << endl;
        do_close();

        return;
    }

    if (bytesTransfered < currentHeader_.size())
    {
        cout << LogType::ERROR << "Invalid packet received: size of packet < size from header" << endl;
        do_close();

        return;
    }

    socket_.async_receive(buffer(currentHeader_.data(), 6), bind(&NetworkClient::handlePacketHeader, this, std::placeholders::_1, std::placeholders::_2));
}

void NetworkClient::do_close()
{
    cout << LogType::DEBUG << "Closing socket..." << endl;

    socket_.close();

    cout << LogType::NORMAL << "Client socket closed" << endl;
}



ip::tcp::socket& NetworkClient::socket()
{
    return socket_;
}
