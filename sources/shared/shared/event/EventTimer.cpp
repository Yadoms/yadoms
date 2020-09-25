#include "stdafx.h"
#include "../currentTime/Provider.h"
#include "EventTimer.h"

namespace shared
{
   namespace event
   {
      CEventTimer::CEventTimer(int eventId,
                               EPeriodicity periodicity,
                               const boost::posix_time::time_duration& period)
         : m_id(eventId),
           m_periodicity(periodicity),
           m_period(period),
           m_nextStopPoint(boost::date_time::not_a_date_time),
           m_isRunning(false)
      {
         if (m_period != boost::date_time::not_a_date_time)
            start(m_period);
      }

      void CEventTimer::start(const boost::posix_time::time_duration& period)
      {
         // Starting an already running periodic timer makes no sense
         if (m_periodicity == kPeriodic && m_isRunning)
            return;

         doStart(period);

         m_isRunning = true;
      }

      void CEventTimer::stop()
      {
         m_nextStopPoint = boost::date_time::not_a_date_time;
         m_isRunning = false;
      }

      bool CEventTimer::isRunning() const
      {
         return m_isRunning;
      }

      void CEventTimer::doStart(const boost::posix_time::time_duration& period)
      {
         // Use provided period if available, initialize period else
         if (period == boost::date_time::not_a_date_time && m_period == boost::date_time::not_a_date_time)
            throw std::invalid_argument("no period value was provided to start timer");
         const auto& periodToUse = (period != boost::date_time::not_a_date_time) ? period : m_period;

         // If periodic (and not the first time), next start point begins from last start point
         // Else next start point begins from now
         const auto startPoint = (m_periodicity == kPeriodic && m_nextStopPoint != boost::date_time::not_a_date_time)
                                    ? m_nextStopPoint
                                    : currentTime::Provider().now();

         m_nextStopPoint = startPoint + periodToUse;
      }

      boost::posix_time::ptime CEventTimer::getNextStopPoint() const
      {
         return m_nextStopPoint;
      }

      void CEventTimer::reset()
      {
         if (m_periodicity == kPeriodic)
            doStart();
         else
            stop();
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
