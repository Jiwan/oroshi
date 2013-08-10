#ifndef LOGINCOREEGNINE_HPP
#define LOGINCOREENGINE_HPP

#include <thread>
#include <vector>
#include <boost/asio.hpp>

#include <common/core/core-engine.hpp>

#include "login-event.hpp"

namespace oroshi
{
    namespace login
    {
        // If this inheritance seam weird, take a look at the CRPT (Curiously recurring template pattern) pattern.
        template <int threadCount> class LoginCoreEngine: 
        public oroshi::common::core::CoreEngine<LoginCoreEngine<threadCount>, threadCount>
        {  
        public:
            // All handler must have their events by value.
            void doHandle(UserLoginEvent event)
            {
                    std::cout << LogType::LOG_DEBUG << "account: "  << event.account() << std::endl;
                    std::cout << LogType::LOG_DEBUG << "password: " << event.password() << std::endl;
            }
        };
    }
}

#endif // LOGINCOREENGINE_HPP