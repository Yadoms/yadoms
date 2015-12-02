#include "stdafx.h"
#include "TimeAdapter.h"

namespace automation { namespace script
{

CTimeAdapter::CTimeAdapter(const std::string& time)
{
   convert(time);
}

CTimeAdapter::~CTimeAdapter()
{
}

void CTimeAdapter::convert(const std::string& time)
{
   try
   {
      m_duration = boost::posix_time::duration_from_string(time);
      m_isDateTime = false;
      return;
   }
   catch (boost::bad_lexical_cast&)
   {      
   }

   try
   {
      m_dateTime = boost::posix_time::time_from_string(time);
      m_isDateTime = true;
      return;
   }
   catch (boost::bad_lexical_cast&)
   {
   }

   throw std::out_of_range("Can not interpret provided time : is not a dateTime or duration");
}

bool CTimeAdapter::isDateTime() const
{
   return m_isDateTime;
}

boost::posix_time::time_duration CTimeAdapter::duration() const
{
   if (m_isDateTime)
      throw std::out_of_range("Read duration from a string : string is not a duration");
   return m_duration;
}

boost::posix_time::ptime CTimeAdapter::dateTime() const
{
   if (!m_isDateTime)
      throw std::out_of_range("Read dateTime from a string : string is not a dateTime");
   return m_dateTime;
}
  
} } // namespace automation::script
	
	