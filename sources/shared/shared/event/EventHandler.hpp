#pragma once
#include "Event.hpp"
#include "EventTimer.h"
#include "EventTimePoint.h"
#include <shared/currentTime/Provider.h>

namespace shared
{
   namespace event
   {
      //--------------------------------------------------------------
      /// \brief	    Special event id values, used by waitForEvents return value
      //--------------------------------------------------------------
      enum
      {
         kNoEvent = -2,   ///< No event in the queue
         kTimeout = -1,   ///< Timeout
         kUserFirstId = 0 ///< Base of user-defined IDs
      };

      //--------------------------------------------------------------
      /// \brief	    An handler for events
      //--------------------------------------------------------------
      class CEventHandler
      {
      public:
         CEventHandler() = default;
         virtual ~CEventHandler() = default;

         // Avoid copy
         CEventHandler(const CEventHandler&) = delete;
         const CEventHandler& operator=(const CEventHandler&) = delete;

         // Avoid move
         CEventHandler(const CEventHandler&&) = delete;
         const CEventHandler& operator=(const CEventHandler&&) = delete;

         //--------------------------------------------------------------
         /// \brief	    Send empty event (without data)
         /// \param[in] id Event id
         //--------------------------------------------------------------
         void postEvent(int id)
         {
            auto evt = boost::make_shared<CEventBase>(id);
            postEvent(evt);
         }

         //--------------------------------------------------------------
         /// \brief	    Send event
         /// \template DataType  Type of the data in the event
         /// \param[in] id Event id
         /// \param[in] data Event data
         //--------------------------------------------------------------
         template <typename DataType>
         void postEvent(int id, const DataType& data)
         {
            boost::shared_ptr<CEventBase> evt = boost::make_shared<CEvent<DataType>>(id, data);
            postEvent(evt);
         }

         //--------------------------------------------------------------
         /// \brief	    Check if event is awaiting
         /// \return     true is event is present in the queue
         /// \note       Equivalent to a call to waitForEvents(min_date_time)
         //--------------------------------------------------------------
         bool empty() const
         {
            boost::recursive_mutex::scoped_lock lock(m_eventsQueueMutex);
            return m_eventsQueue.empty();
         }

         //--------------------------------------------------------------
         /// \brief	    Erase all pending events
         //--------------------------------------------------------------
         void clear()
         {
            boost::recursive_mutex::scoped_lock lock(m_eventsQueueMutex);
            while (!m_eventsQueue.empty())
               m_eventsQueue.pop();
         }

         //--------------------------------------------------------------
         /// \brief	    wait for an event
         /// \param[in] timeout  Set an optional timeout delay. Can use these special values :
         ///             - pos_infin : no timeout
         ///             - min_date_time : no wait, function returns immediately
         ///             is set to min_date_time special value.
         /// \return     ID of the received event, or kTimeout if timeout was reached, or kNoEvent if no event is available
         /// \throw      boost::thread_interrupted if thread was interrupted
         /// \note       This function is blocking (if timeout is different to min_date_time).
         ///             This blocking state will be brake when thread is interrupted.
         //--------------------------------------------------------------
         int waitForEvents(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin)
         {
            // Prior check if thread stopping was requested
            boost::this_thread::interruption_point();

            // Clean last received message data
            m_lastEvent.reset();

            // Clean the time event list
            timeEventListCleanup();

            // If time events are elapsed, must post corresponding event to the queue
            signalElapsedTimeEvents();

            boost::recursive_mutex::scoped_lock lock(m_eventsQueueMutex);

            // Don't wait if event is already present
            if (!m_eventsQueue.empty())
               return popEvent();

            // No event is currently present
            if (timeout == boost::date_time::min_date_time)
            {
               // No wait
               return kNoEvent;
            }

            if (!hasRunningTimeEvents() && timeout == boost::date_time::pos_infin)
            {
               // Wait infinite for event
               m_condition.wait(lock);
               return popEvent();
            }

            // Have time event or timeout
            const auto closerTimeEvent = getNextTimeEventStopPoint();
            if (!!closerTimeEvent && (closerTimeEvent->getNextStopPoint() < (currentTime::Provider().now() + timeout)))
            {
               // Next stop point will be the closer time event
               if (!m_condition.timed_wait(lock, closerTimeEvent->getNextStopPoint() - currentTime::Provider().now()))
               {
                  // No event ==> Signal time event
                  signalTimeEvent(closerTimeEvent);
               }
            }
            else
            {
               // Next stop point will be the normal timeout
               if (!m_condition.timed_wait(lock, timeout))
               {
                  // No event ==> timeout
                  return kTimeout;
               }
            }

            // Event occurs during wait or time event was signaled
            return popEvent();
         }

         //--------------------------------------------------------------
         /// \brief	    Get the last event ID
         /// \return     event ID of the last event
         /// \throw      std::runtime_error if no event is available
         /// \note       Must be called after waitForEvents
         //--------------------------------------------------------------
         int getEventId() const
         {
            if (!m_lastEvent)
               throw std::runtime_error("getEventId, no event available");

            return m_lastEvent->getId();
         }

         //--------------------------------------------------------------
         /// \brief	    Check if the last event is the type
         /// \template DataType  Type of the data to check
         /// \return     true if event data is type of DataType
         /// \throw      std::runtime_error if no event data is available
         /// \note       Must be called after waitForEvents
         //--------------------------------------------------------------
         template <typename DataType>
         bool isEventType() const
         {
            if (!m_lastEvent)
               throw std::runtime_error("isEventType, no event available");

            try
            {
               auto evt = boost::dynamic_pointer_cast<CEvent<DataType>>(m_lastEvent);
               return evt.get() != NULL;
            }
            catch (std::bad_cast&)
            {
               return false;
            }
         }

         //--------------------------------------------------------------
         /// \brief	    Get data from last event
         /// \template DataType  Type of the data in the event
         /// \return     Copy of event data
         /// \throw      std::runtime_error if event data is not of correct type or no event data is available
         /// \throw      std::bad_cast if event data is not of correct type
         /// \note       Must be called after waitForEvents
         //--------------------------------------------------------------
         template <typename DataType>
         DataType getEventData() const
         {
            if (!m_lastEvent)
               throw std::runtime_error("getEventData, no event available");

            CEvent<DataType> evt = dynamic_cast<CEvent<DataType>&>(*m_lastEvent);
            return evt.getData();
         }

         //--------------------------------------------------------------
         /// \brief	    Transfer last event to another EventHandler
         /// \param [in] rhs The event handler to transfer the event
         /// \note       Must be called after waitForEvents
         //--------------------------------------------------------------
         void transferLastEvent(CEventHandler& rhs)
         {
            if (m_lastEvent)
               rhs.postEvent(m_lastEvent);
         }

         //--------------------------------------------------------------
         /// \brief	    Create timer associated with this event handler
         /// \param[in] timerEventId   Id of the timer event (must be >= kUserFirstId)
         /// \param[in] periodicity    Periodic or one-shot timer
         /// \param[in] period         Timer period. If provided, timer starts immediately, else user must call start method
         /// \return     the created timer (see note)
         /// \throw      std::invalid_argument if timerEventId uses reserved values
         /// \note       Usually, caller don't need to get the timer object as it is owned (and will be destroyed) by the event handler.
         //              Keep a reference on the timer object can be useful if you want to re-use it or differ start. In this case,
         //              the event handler won't remove it from it's time events list.
         //--------------------------------------------------------------
         boost::shared_ptr<CEventTimer> createTimer(int timerEventId,
                                                    CEventTimer::EPeriodicity periodicity = CEventTimer::kOneShot,
                                                    const boost::posix_time::time_duration& period = boost::date_time::
                                                       not_a_date_time)
         {
            if (timerEventId < kUserFirstId)
               throw std::invalid_argument("CEventHandler::createTimer : invalid ID " + std::to_string(timerEventId));

            auto timer(boost::make_shared<CEventTimer>(timerEventId,
                                                       periodicity,
                                                       period));

            m_timeEvents.push_back(timer);
            return timer;
         }

         //--------------------------------------------------------------
         /// \brief	    Create time point event associated with this event handler
         /// \param[in] timePointEventId  Id of the time point event (must be >= kUserFirstId)
         /// \param[in] dateTime          date/time when to raise the event, must be in the future
         /// \return     the created time point (see note)
         /// \throw      std::invalid_argument if timePointEventId uses reserved values or dateTime not in the future
         /// \note       - Usually, caller don't need to get the time point object as it is owned (and will be destroyed) by the event handler.
         //              Keep a reference on the time point object can be useful if you want to re-use it or differ initialization. In this case,
         //              the event handler won't remove it from it's time events list.
         //              - Be careful of a limitation : a time-point is not DST (Daylight Saving Time) compatible : for example, 
         //              a timepoint set after a DST change (ie change to summer hour) will not take account of time offset.
         //--------------------------------------------------------------
         boost::shared_ptr<CEventTimePoint> createTimePoint(int timePointEventId,
                                                            const boost::posix_time::ptime& dateTime)
         {
            if (timePointEventId < kUserFirstId)
               throw std::invalid_argument("CEventHandler::createTimePoint : invalid ID " + std::to_string(timePointEventId));
            if (dateTime <= currentTime::Provider().now())
               throw std::invalid_argument("CEventHandler::createTimePoint : timePoint not in the future");

            auto timePoint(boost::make_shared<CEventTimePoint>(timePointEventId,
                                                               dateTime));

            m_timeEvents.push_back(timePoint);
            return timePoint;
         }


      private:
         //--------------------------------------------------------------
         /// \brief	    Send an event
         /// \param[in] event event to send
         //--------------------------------------------------------------
         void postEvent(boost::shared_ptr<CEventBase>& event)
         {
            boost::recursive_mutex::scoped_lock lock(m_eventsQueueMutex);
            pushEvent(event);
            m_condition.notify_one();
         }

         //--------------------------------------------------------------
         /// \brief	    Push an event to the queue
         /// \param[in] event event to push
         //--------------------------------------------------------------
         void pushEvent(boost::shared_ptr<CEventBase>& event)
         {
            if (event->getId() < kUserFirstId)
               throw std::invalid_argument("CEventHandler::pushEvent : invalid ID " + std::to_string(event->getId()));

            boost::recursive_mutex::scoped_lock lock(m_eventsQueueMutex);
            m_eventsQueue.push(event);
         }

         //--------------------------------------------------------------
         /// \brief	    Pop the next event
         /// \return     ID of the next event
         //--------------------------------------------------------------
         int popEvent()
         {
            boost::recursive_mutex::scoped_lock lock(m_eventsQueueMutex);

            if (m_eventsQueue.empty())
            {
               //sometimes happen with MacOSX, don't know why
               return kNoEvent;
            }

            //good behavior
            m_lastEvent = m_eventsQueue.front();
            m_eventsQueue.pop();
            return m_lastEvent->getId();
         }

         //--------------------------------------------------------------
         /// \brief	   Get the next time event stop point
         ///            This function computes the next event to arrive, between registered time events
         /// \return    The next time event (null pointer if none)
         //--------------------------------------------------------------
         boost::shared_ptr<ITimeEvent> getNextTimeEventStopPoint() const
         {
            if (m_timeEvents.empty())
               return boost::shared_ptr<ITimeEvent>();

            // Find the closer time event
            boost::posix_time::ptime lower = boost::posix_time::max_date_time;
            boost::shared_ptr<ITimeEvent> nextTimeEvent;
            for (const auto& timeEvent : m_timeEvents)
            {
               const auto nextStopPoint = timeEvent->getNextStopPoint();
               if (nextStopPoint != boost::date_time::not_a_date_time)
               {
                  if (nextStopPoint < lower)
                  {
                     lower = nextStopPoint;
                     nextTimeEvent = timeEvent;
                  }
               }
            }
            return nextTimeEvent;
         }

         //--------------------------------------------------------------
         /// \brief	   Check if some time event(s) is(are) running
         /// \return    true if time event is running, false if none
         //--------------------------------------------------------------
         bool hasRunningTimeEvents()
         {
            if (m_timeEvents.empty())
               return false;

            return std::any_of(m_timeEvents.begin(),
                               m_timeEvents.end(),
                               [](const auto& timeEvent)
                               {
                                  return timeEvent->getNextStopPoint() != boost::date_time::not_a_date_time;
                               });
         }

         //--------------------------------------------------------------
         /// \brief	   Check for elapsed time events, and post corresponding events to the queue
         //--------------------------------------------------------------
         void signalElapsedTimeEvents()
         {
            if (m_timeEvents.empty())
               return;

            for (const auto& timeEvent : m_timeEvents)
            {
               const auto nextStopPoint = timeEvent->getNextStopPoint();
               if (nextStopPoint != boost::date_time::not_a_date_time && nextStopPoint < currentTime::Provider().now())
                  signalTimeEvent(timeEvent); // Elapsed time point, signal it
            }
         }

         //--------------------------------------------------------------
         /// \brief	            Signal that time event elapsed
         /// \param[in] timeEvent    Time event to signal
         //--------------------------------------------------------------
         void signalTimeEvent(const boost::shared_ptr<ITimeEvent>& timeEvent)
         {
            if (!timeEvent)
               throw std::invalid_argument("CEventHandler::signalTimeEvent : timeEvent is null");

            auto evt(boost::make_shared<CEventBase>(timeEvent->getId()));
            pushEvent(evt);
            timeEvent->reset();
         }

         //--------------------------------------------------------------
         /// \brief	            Purge obsolete time events
         //--------------------------------------------------------------
         void timeEventListCleanup()
         {
            if (m_timeEvents.empty())
               return;

            for (auto it = m_timeEvents.begin();
                 it != m_timeEvents.end();)
            {
               if ((*it).unique() && (*it)->canBeRemoved())
               {
                  // Time event no more makes sense, and is not referenced by user, so erase it from the list
                  it = m_timeEvents.erase(it);
               }
               else
               {
                  ++it;
               }
            }
         }


         //--------------------------------------------------------------
         /// \brief	   The events queue
         //--------------------------------------------------------------
         std::queue<boost::shared_ptr<CEventBase>> m_eventsQueue;

         //--------------------------------------------------------------
         /// \brief	   The last received event
         //--------------------------------------------------------------
         boost::shared_ptr<CEventBase> m_lastEvent;

         //--------------------------------------------------------------
         /// \brief	   Mutex protecting the events queue
         //--------------------------------------------------------------
         mutable boost::recursive_mutex m_eventsQueueMutex;

         //--------------------------------------------------------------
         /// \brief	   Condition variable signaling an event arrives
         //--------------------------------------------------------------
         boost::condition_variable_any m_condition;

         //--------------------------------------------------------------
         /// \brief	   The time events associated with this event handler
         //--------------------------------------------------------------
         typedef std::vector<boost::shared_ptr<ITimeEvent>> TimeEventList;
         TimeEventList m_timeEvents;
      };
   }
} // namespace shared::event
