#pragma once
#include "stdafx.h"
#include "../exception/InvalidParameter.hpp"
#include "Now.h"
#include "EventTimePoint.h"

namespace shared { namespace event
{

CEventTimePoint::CEventTimePoint(int eventId, const boost::posix_time::ptime& dateTime)
   :m_dateTime(dateTime), m_id(eventId)
{
   if (m_dateTime <= now())
      throw shared::exception::CInvalidParameter("Provided dateTime value is not in the future");
}

CEventTimePoint::~CEventTimePoint()
{
}

boost::posix_time::ptime CEventTimePoint::getNextStopPoint() const
{
   return m_dateTime;
}

void CEventTimePoint::reset()
{
   m_dateTime = boost::date_time::not_a_date_time;
}

bool CEventTimePoint::canBeDetached() const
{
   return (m_dateTime == boost::date_time::not_a_date_time);
}

int CEventTimePoint::getId() const
{
   return m_id;
}

} } // namespace shared::event
