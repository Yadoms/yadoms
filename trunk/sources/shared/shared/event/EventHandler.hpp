#pragma once
#include "Event.hpp"
#include <shared/exceptions/BadConversionException.hpp>

namespace shared { namespace event
{

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
         boost::mutex::scoped_lock lock(m_EventsQueueMutex);
         return m_EventsQueue.empty();
      }

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
         boost::mutex::scoped_lock lock(m_EventsQueueMutex);

         // Don't wait if event is already present
         if (!m_EventsQueue.empty())
            return m_EventsQueue.back()->getId();

         // No event is present
         if (timeout == boost::date_time::min_date_time)
         {
            // No wait
            return kNoEvent;
         }
         else if (timeout == boost::date_time::pos_infin)
         {
            // Wait inifinite for event
            m_condition.wait(lock);
         }
         else
         {
            // Wait for event with timeout
            if (!m_condition.timed_wait(lock, timeout))
               return kTimeout;
         }
         return m_EventsQueue.back()->getId();
      }

      //--------------------------------------------------------------
      /// \brief	    Pop the next event
      //--------------------------------------------------------------
      void popEvent()
      {
         boost::mutex::scoped_lock lock(m_EventsQueueMutex);
         m_EventsQueue.pop();
      }

      //--------------------------------------------------------------
      /// \brief	    Get and pop the next event
      /// \template DataType  Type of the data in the event
      /// \return     const reference on Event data (caller have to copy data if it want to keep them)
      /// \throw      CBadConversionException if event data is not correct
      //--------------------------------------------------------------
      template<typename DataType>
      const DataType popEvent()
      {
         boost::mutex::scoped_lock lock(m_EventsQueueMutex);
         try
         {
            CEvent<DataType> evt = dynamic_cast<CEvent<DataType> & >(*m_EventsQueue.front());
            m_EventsQueue.pop();
            return evt.getData();
         }
         catch (std::bad_cast&)
         {
            throw CBadConversionException("popEvent", boost::lexical_cast<std::string>(m_EventsQueue.back()->getId()));
         }
      }

   protected:
      //--------------------------------------------------------------
      /// \brief	    Send an event
      /// \param[in] event event to send
      //--------------------------------------------------------------
      void sendEvent(boost::shared_ptr<CEventBase> & event)
      {
         BOOST_ASSERT(event->getId() >= kUserFirstId);

         boost::shared_ptr<CEventBase> evt(event);

         boost::mutex::scoped_lock lock(m_EventsQueueMutex);
         m_EventsQueue.push(evt);
         lock.unlock();
         m_condition.notify_one();
      }

   private:
      //--------------------------------------------------------------
      /// \brief	   The events queue
      //--------------------------------------------------------------
      std::queue<boost::shared_ptr<CEventBase> > m_EventsQueue;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the events queue
      //--------------------------------------------------------------
      mutable boost::mutex m_EventsQueueMutex;

      //--------------------------------------------------------------
      /// \brief	   Condition variable signaling an event arrives
      //--------------------------------------------------------------
      boost::condition_variable m_condition;
   };

} } // namespace shared::event
