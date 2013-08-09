#ifndef LOGINCOREEGNINE_HPP
#define LOGINCOREENGINE_HPP

#include <thread>
#include <vector>
#include <boost/asio.hpp>

#include "login-event.hpp"

namespace oroshi
{
    namespace login
    {
        class LoginCoreEngine
        {
        public:
            void start(int threadCount)
            {
                for (int i = 0; i < threadCount; ++i)
                {
                    auto workerThread = std::thread(std::bind(static_cast<std::size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &ioService_));

                    workerThreads_.push_back(std::move(workerThread));
                }
            }

            void stop()
            {
                for (auto& workerThread : workerThreads_)
                {
                    workerThread.join();
                }    
            }

            template <class EventType> void handle(EventType& type)
            {
                ioService_.post(std::bind(&LoginCoreEngine::doHandle, this, type));
            }

        private:
            // All handler must have their events by value.
            void doHandle(UserLoginEvent event)
            {
                std::cout << event.account() <<
            }

        private:
            boost::asio::io_service ioService_;
            std::vector<std::thread> workerThreads_;
        };
    }
}

#endif // LOGINCOREENGINE_HPP