#ifndef LOGINDATABASEEGNINE_HPP
#define LOGINDATABASEEGNINE_HPP

#include <common/core/core-engine.hpp>

#include <mongo/client/dbclient.h>

namespace oroshi
{
    namespace login
    {
        template <int threadCount> class LoginDatabaseEngine: 
        public oroshi::common::core::CoreEngine<LoginCoreEngine<threadCount>, threadCount>
        {

        };
    }
}

#endif