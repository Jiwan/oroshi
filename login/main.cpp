#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <common/network/network-engine.hpp>
#include <common/utils/log.hpp>
#include <common/network/basic-packet-crypt.hpp>

#include "login-packet-handler.hpp"

using namespace oroshi::common::network;
using namespace oroshi::common::utils;
using namespace oroshi::login;

CurrentLoginCoreEngine gCoreEngine;

int main(int argc, char* argv[])
{
    std::cout << LogType::LOG_NORMAL << "Starting server..." << std::endl;

    NetworkEngine<LoginPacketHandler, BasicPacketCrypt, CurrentLoginCoreEngine> networkEngine(gCoreEngine);

    gCoreEngine.start();
    networkEngine.start("127.0.0.1", 29000);

    networkEngine.stop();
    gCoreEngine.stop();

    return EXIT_SUCCESS;
}
