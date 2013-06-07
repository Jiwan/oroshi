#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <common/network/network-engine.hpp>
#include <common/utils/log.hpp>

#include "login-packet-handler.hpp"

using namespace oroshi::common::network;
using namespace oroshi::common::utils;

int main(int argc, char* argv[])
{
    std::cout << LogType::NORMAL << "Starting server..." << std::endl;

    NetworkEngine<oroshi::login::LoginPacketHandler> engine;

    engine.start();

    engine.stop();
    
    return EXIT_SUCCESS;
}
