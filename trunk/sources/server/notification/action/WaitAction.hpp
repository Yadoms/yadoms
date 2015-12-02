#pragma once

#include "EventAction.hpp"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace action {
   
   //-----------------------------
   ///\brief Provide a notification action using self contained event handler
   ///\template T The notification content type
   //-----------------------------
   template<class T>
   class CWaitAction : public CEventAction< T >
   {
   public:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------
      CWaitAction()
         :CEventAction< T >(m_eventHandler, shared::event::kUserFirstId)
      {
      }

      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~CWaitAction()
      {
      }

      //-----------------------------------------------------
      ///\brief               Wait for simple acquisition (without summaries)
      ///\param[in] timeout  Set an optional timeout delay. Can use these special values :
      ///             - pos_infin : no timeout
      ///             - min_date_time : no wait, function returns immediately
      ///             is set to min_date_time special value.
      ///\return the new acquisition, null pointer if timeout
      //-----------------------------------------------------
      virtual boost::shared_ptr< T > wait(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin)
      {
         if (m_eventHandler.waitForEvents(timeout) == shared::event::kTimeout)
            return boost::shared_ptr< T >();

         return m_eventHandler.getEventData<boost::shared_ptr< T > >();
      }

      //-----------------------------------------------------
      ///\brief               Wait for simple acquisition (without summaries)
      ///\param[in] timePoint Timeout is here expressed as timePoint
      //-----------------------------------------------------
      virtual boost::shared_ptr< T > wait(const boost::posix_time::ptime& timePoint)
      {
         enum { kTimePointEventId = shared::event::kUserFirstId + 1 };
         m_eventHandler.createTimePoint(kTimePointEventId, timePoint);

         if (m_eventHandler.waitForEvents() == kTimePointEventId)
            return boost::shared_ptr< T >();

         return m_eventHandler.getEventData<boost::shared_ptr< T > >();
      }


   private:
      //-----------------------------
      ///\brief Event handler
      //-----------------------------
      shared::event::CEventHandler m_eventHandler;
   };

} // namespace action
} // namespace notification
