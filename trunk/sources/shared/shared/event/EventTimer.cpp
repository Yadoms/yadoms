#pragma once
#include "stdafx.h"
#include "../exception/InvalidParameter.hpp"
#include "Now.h"
#include "EventTimer.h"

namespace shared { namespace event
{

CEventTimer::CEventTimer(int timerEventId, bool periodic, const boost::posix_time::time_duration& period)
   :m_periodic(periodic), m_period(period), m_id(timerEventId)
{
   if (m_period != boost::date_time::not_a_date_time)
      start(m_period);
}

CEventTimer::~CEventTimer()
{
}

void CEventTimer::start(const boost::posix_time::time_duration& period)
{
   // Use provided period is available, initialisation period else
   if (period == boost::date_time::not_a_date_time && m_period == boost::date_time::not_a_date_time)
      throw shared::exception::CInvalidParameter("no period value was provided to start timer");
   const boost::posix_time::time_duration& periodToUse = (period != boost::date_time::not_a_date_time) ? period : m_period;

   // If periodic (and not the first time), next start point begins from last start point
   // Else next start point begins from now
   boost::posix_time::ptime startPoint = (m_periodic && m_nextStopPoint != boost::date_time::not_a_date_time) ? m_nextStopPoint : now();

   m_nextStopPoint = startPoint + periodToUse;
}

boost::posix_time::ptime CEventTimer::getNextStopPoint() const
{
   return m_nextStopPoint;
}

void CEventTimer::reset()
{
   if (m_periodic)
      start();
   else
      m_nextStopPoint = boost::date_time::not_a_date_time;
}

int CEventTimer::getId() const
{
   return m_id;
}

} } // namespace shared::event
