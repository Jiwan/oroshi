#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

namespace oroshi
{
    namespace common
    {
        // In case we wants to do a multi dispatch to obtain the dynamic type of an event.
        template <class EventHandler> class Event
        {
            virtual void accept(EventHandler& handler) = 0;
        };

        /**
         * For instance:
         * template <class EventHandler> class YourEvent: Event<EventHandler>
         * { 
         *     virtual void accept(EventHandler& handler) override
         *     {
         *          handler.handle(*this); // (1): Will jump to (2)
         *     }  
         * };
         *
         *
         * class YourHandler
         * {
         *     void handle(Event<YourHandler>* event)
         *     {
         *         event->accept(*this); // Will jump to (1)
         *     }
         *
         *    void handle(YourEvent<YourHandler>& event)
         *    {
         *      // (2)
         *    }
         * };
         *
         * This avoid to do some nasty dynamic_cast to find the type of the Event, but is slower due to virtual table call.
         * In most cases, you already know the type of the event as you are the emitter. 
         * In this case, you must use templates to avoid the dynamic computation (see the LoginCore) .
         */
    }
}
#endif