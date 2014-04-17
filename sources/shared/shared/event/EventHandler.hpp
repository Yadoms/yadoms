#pragma once
#include "Event.hpp"
#include "EventTimer.h"
#include "Now.h"
#include <shared/exception/BadConversion.hpp>

namespace shared { namespace event
{
   //--------------------------------------------------------------
   /// \brief	    Special event id values, used by waitForEvents return value
   //--------------------------------------------------------------
   enum
   {
      kNoEvent = -2,       // No event in the queue
      kTimeout = -1,       // Timeout
      kUserFirstId = 0     // Base of user-defined IDs
   };

   //--------------------------------------------------------------
   /// \brief	    An handler for events
   //--------------------------------------------------------------
   class CEventHandler
   {  
   public:
      CEventHandler() {}

      virtual ~CEventHandler() {}

      //--------------------------------------------------------------
      /// \brief	    Send empty event (without data)
      /// \param[in] id Event id
      //--------------------------------------------------------------
      void sendEvent(int id)
      {
         boost::shared_ptr<CEventBase> evt(new CEventBase(id));
         sendEvent(evt);
      }

      //--------------------------------------------------------------
      /// \brief	    Send event
      /// \template DataType  Type of the data in the event
      /// \param[in] id Event id
      /// \param[in] data Event data
      //--------------------------------------------------------------
      template<typename DataType>
      void sendEvent(int id, const DataType& data)
      {
         boost::shared_ptr<CEventBase> evt(new CEvent<DataType>(id, data));
         sendEvent(evt);
      }

      //--------------------------------------------------------------
      /// \brief	    Check if event is awaiting
      /// \return     true is event is present in the queue
      /// \note       Equivalent to a call to waitForEvents(min_date_time)
      //--------------------------------------------------------------
      bool empty() const
      {
         boost::mutex::scoped_lock lock(m_eventsQueueMutex);
         return m_eventsQueue.empty();
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
         // If time events are elapsed, must post corresponding event to the queue
         signalElapsedTimeEvents();

         boost::mutex::scoped_lock lock(m_eventsQueueMutex);

         // Don't wait if event is already present
         if (!m_eventsQueue.empty())
            return m_eventsQueue.back()->getId();

         // No event is currently present
         if (timeout == boost::date_time::min_date_time)
         {
            // No wait
            return kNoEvent;
         }
         else if (!hasRunningTimeEvents() && timeout == boost::date_time::pos_infin)
         {
            // Wait inifinite for event
            m_condition.wait(lock);
            return m_eventsQueue.back()->getId();
         }
         else
         {
            // Have time event or timeout
            boost::shared_ptr<ITimeEvent> closerTimeEvent = getNextTimeEventStopPoint();
            if (closerTimeEvent && (closerTimeEvent->getNextStopPoint() < (now() + timeout)) )
            {
               // Next stop point will be the closer time event
               if (!m_condition.timed_wait(lock, closerTimeEvent->getNextStopPoint() - now()))
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
            return m_eventsQueue.back()->getId();
         }
      }

      //--------------------------------------------------------------
      /// \brief	    Pop the next event
      //--------------------------------------------------------------
      void popEvent()
      {
         boost::mutex::scoped_lock lock(m_eventsQueueMutex);
         m_eventsQueue.pop();
      }

      //--------------------------------------------------------------
      /// \brief	    Get and pop the next event
      /// \template DataType  Type of the data in the event
      /// \return     const reference on Event data (caller have to copy data if it want to keep them)
      /// \throw      exception::CBadConversion if event data is not correct
      //--------------------------------------------------------------
      template<typename DataType>
      const DataType popEvent()
      {
         boost::mutex::scoped_lock lock(m_eventsQueueMutex);
         try
         {
            CEvent<DataType> evt = dynamic_cast<CEvent<DataType> & >(*m_eventsQueue.front());
            m_eventsQueue.pop();
            return evt.getData();
         }
         catch (std::bad_cast&)
         {
            throw exception::CBadConversion("popEvent", boost::lexical_cast<std::string>(m_eventsQueue.back()->getId()));
         }
      }

      //--------------------------------------------------------------
      /// \brief	    Create timer associated with this event handler
      /// \param[in] timerEventId   Id of the timer event
      /// \param[in] periodic       true if the timer is periodic, false if timer is one-shot
      /// \param[in] period         Timer period. If provided, timer starts immediatley, else user must call start method
      //--------------------------------------------------------------
      void createTimer(int timerEventId, bool periodic = false,
         const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time)
      {
         BOOST_ASSERT(timerEventId >= kUserFirstId);

         boost::shared_ptr<ITimeEvent> timer(new CEventTimer(timerEventId, periodic, period));
         m_timeEvents.push_back(timer);
      }

   protected:
      //--------------------------------------------------------------
      /// \brief	    Send an event
      /// \param[in] event event to send
      //--------------------------------------------------------------
      void sendEvent(boost::shared_ptr<CEventBase> & event)
      {
         BOOST_ASSERT(event->getId() >= kUserFirstId);

//         boost::shared_ptr<CEventBase> evt(event);//TODO : evt est-il nécessaire ? Ne peut-on pas poster directement event ?

         boost::mutex::scoped_lock lock(m_eventsQueueMutex);
         m_eventsQueue.push(event);
         lock.unlock();
         m_condition.notify_one();
      }

      //--------------------------------------------------------------
      /// \brief	   Get the next time event stop point
      ///            This function compute the next event to arrive, between registred time events
      /// \return    The next time event (null pointer if none)
      //--------------------------------------------------------------
      boost::shared_ptr<ITimeEvent> getNextTimeEventStopPoint()
      {
         if (m_timeEvents.empty())
            return boost::shared_ptr<ITimeEvent>();

         // Find the closer time event
         boost::posix_time::ptime lower = boost::posix_time::max_date_time;
         boost::shared_ptr<ITimeEvent> nextTimeEvent;
         for (std::vector<boost::shared_ptr<ITimeEvent> >::const_iterator it = m_timeEvents.begin() ;
            it != m_timeEvents.end() ; ++it)
         {
            boost::posix_time::ptime nextStopPoint = (*it)->getNextStopPoint();
            if (nextStopPoint != boost::date_time::not_a_date_time)
            {
               if (nextStopPoint < lower)
               {
                  lower = nextStopPoint;
                  nextTimeEvent = *it;
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

         for (std::vector<boost::shared_ptr<ITimeEvent> >::const_iterator it = m_timeEvents.begin() ;
            it != m_timeEvents.end() ; ++it)
         {
            if ((*it)->getNextStopPoint() != boost::date_time::not_a_date_time)
               return true;
         }

         return false;
      }

      //--------------------------------------------------------------
      /// \brief	   Check for elapsed time events, and post corresponding events to the queue
      //--------------------------------------------------------------
      void signalElapsedTimeEvents()
      {
         if (m_timeEvents.empty())
            return;

         for (std::vector<boost::shared_ptr<ITimeEvent> >::const_iterator it = m_timeEvents.begin() ;
            it != m_timeEvents.end() ; ++it)
         {
            boost::posix_time::ptime nextStopPoint = (*it)->getNextStopPoint();
            if (nextStopPoint != boost::date_time::not_a_date_time && nextStopPoint < now())
               signalTimeEvent(*it);
         }
      }

      //--------------------------------------------------------------
      /// \brief	            Signal that time event elapsed
      /// \param[in] timeEvent    Time event to signal
      //--------------------------------------------------------------
      void signalTimeEvent(boost::shared_ptr<ITimeEvent> timeEvent)
      {
         boost::shared_ptr<CEventBase> evt(new CEventBase(timeEvent->getId()));
         m_eventsQueue.push(evt);
         timeEvent->reset();
      }

   private:
      //--------------------------------------------------------------
      /// \brief	   The events queue
      //--------------------------------------------------------------
      std::queue<boost::shared_ptr<CEventBase> > m_eventsQueue;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the events queue
      //--------------------------------------------------------------
      mutable boost::mutex m_eventsQueueMutex;

      //--------------------------------------------------------------
      /// \brief	   Condition variable signaling an event arrives
      //--------------------------------------------------------------
      boost::condition_variable m_condition;

      //--------------------------------------------------------------
      /// \brief	   The time events associated with this event handler
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<ITimeEvent> > m_timeEvents;
   };

} } // namespace shared::event
