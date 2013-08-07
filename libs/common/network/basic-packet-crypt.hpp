#ifndef BASICPACKETCRYPT_HPP
#define BASICPACKETCRYPT_HPP

#include "packet.hpp"

namespace oroshi
{
    namespace common
    {
        namespace network
        {

            class BasicPacketCrypt
            {
            public:
                void decrypt(Packet& packet)
                {
                    auto header = packet.header();
                    auto body	= packet.body();

                    for (uint8_t i = 2; i < 6; ++i)
                    {
                        header->data()[i] ^= 0x61;
                    }

                    for (uint16_t i = 0; i < header->bodySize() - 2; ++i)
                    {
                        body.get()[i] ^= 0x61;
                    }
                }
            };

        }
    }
}
#endif