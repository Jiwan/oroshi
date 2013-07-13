#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <common/network/network-engine.hpp>
#include <common/utils/log.hpp>
#include <common/network/basic-packet-crypt.hpp>

#include "login-packet-handler.hpp"

using namespace oroshi::common::network;
using namespace oroshi::common::utils;

int main(int argc, char* argv[])
{
    std::cout << LogType::LOG_NORMAL << "Starting server..." << std::endl;

	NetworkEngine<oroshi::login::LoginPacketHandler, oroshi::common::network::BasicPacketCrypt> engine;

    engine.start("127.0.0.1", 29000);

    engine.stop();
    
    return EXIT_SUCCESS;
}
