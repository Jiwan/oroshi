#ifndef COREENGINE_HPP
#define COREENGINE_HPP

#include <boost/asio.hpp>

namespace oroshi
{
    namespace common
    {
        namespace core
        {

            template <class ChildEngine, int threadCount = 1> class CoreEngine
            {
            public:
                CoreEngine(): ioService_(), work_(ioService_)
                {

                }

                void start()
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

                template <class EventType> void handle(EventType& event)
                {
                    ioService_.post(std::bind(&ChildEngine::doHandle, static_cast<ChildEngine*>(this), event));
                }

            private:
                boost::asio::io_service ioService_;
                boost::asio::io_service::work work_; // Avoid the io_service to shutdown himself when there is no more works.
                std::vector<std::thread> workerThreads_;
            };

        }
    }
}

#endif // COREENGINE_HPP