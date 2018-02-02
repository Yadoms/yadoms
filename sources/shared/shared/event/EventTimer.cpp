#include "stdafx.h"
#include "../exception/InvalidParameter.hpp"
#include "../currentTime/Provider.h"
#include "EventTimer.h"

namespace shared
{
   namespace event
   {
      CEventTimer::CEventTimer(int eventId, EPeriodicity periodicity,
                               const boost::posix_time::time_duration& period)
         : m_id(eventId),
           m_periodicity(periodicity),
           m_period(period),
           m_nextStopPoint(boost::date_time::not_a_date_time)
      {
         if (m_period != boost::date_time::not_a_date_time)
            start(m_period);
      }

      CEventTimer::~CEventTimer()
      {
      }

      void CEventTimer::start(const boost::posix_time::time_duration& period)
      {
         // Use provided period if available, initialization period else
         if (period == boost::date_time::not_a_date_time && m_period == boost::date_time::not_a_date_time)
            throw exception::CInvalidParameter("no period value was provided to start timer");
         const auto& periodToUse = (period != boost::date_time::not_a_date_time) ? period : m_period;

         // If periodic (and not the first time), next start point begins from last start point
         // Else next start point begins from now
         auto startPoint = (m_periodicity == kPeriodic && m_nextStopPoint != boost::date_time::not_a_date_time)
                              ? m_nextStopPoint
                              : currentTime::Provider().now();

         m_nextStopPoint = startPoint + periodToUse;
      }

      void CEventTimer::stop()
      {
         m_nextStopPoint = boost::date_time::not_a_date_time;
      }

      boost::posix_time::ptime CEventTimer::getNextStopPoint() const
      {
         return m_nextStopPoint;
      }

      void CEventTimer::reset()
      {
         if (m_periodicity == kPeriodic)
            start();
         else
            m_nextStopPoint = boost::date_time::not_a_date_time;
      }

      bool CEventTimer::canBeRemoved() const
      {
         // Timer can be removed if no more event is to come
         return m_nextStopPoint == boost::date_time::not_a_date_time;
      }

      int CEventTimer::getId() const
      {
         return m_id;
      }
   }
} // namespace shared::event


