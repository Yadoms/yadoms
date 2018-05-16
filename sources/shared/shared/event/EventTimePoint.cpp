#include "stdafx.h"
#include "../exception/InvalidParameter.hpp"
#include "../currentTime/Provider.h"
#include "EventTimePoint.h"

namespace shared
{
   namespace event
   {
      CEventTimePoint::CEventTimePoint(int eventId,
                                       const boost::posix_time::ptime& dateTime)
         : m_id(eventId),
           m_dateTime(dateTime)
      {
         if (dateTime == boost::date_time::not_a_date_time)
            return; // Nothing to do more, m_dateTime will be set later by a call to set() method

         set(dateTime);
      }

      CEventTimePoint::~CEventTimePoint()
      {
      }

      void CEventTimePoint::set(const boost::posix_time::ptime& dateTime)
      {
         if (dateTime.is_special() || dateTime <= currentTime::Provider().now())
            throw exception::CInvalidParameter("Provided dateTime value is not valid, or not in the future");
         m_dateTime = dateTime;
      }

      void CEventTimePoint::cancel()
      {
         reset();
      }

      boost::posix_time::ptime CEventTimePoint::getNextStopPoint() const
      {
         return m_dateTime;
      }

      void CEventTimePoint::reset()
      {
         m_dateTime = boost::date_time::not_a_date_time;
      }

      bool CEventTimePoint::canBeRemoved() const
      {
         // Time point can be removed if no more event is to come
         return (m_dateTime == boost::date_time::not_a_date_time);
      }

      int CEventTimePoint::getId() const
      {
         return m_id;
      }
   }
} // namespace shared::event
