#include "stdafx.h"
#include "DateTimeContainer.h"
#include <shared/currentTime/Provider.h>
#include <Poco/LocalDateTime.h>

namespace shared { namespace dateTime {

   CDateTimeContainer::CDateTimeContainer()
      :m_time(shared::currentTime::Provider().now())
   {
   }

   CDateTimeContainer::CDateTimeContainer(boost::posix_time::ptime & time)
      :m_time(time)
   {
   }

   CDateTimeContainer::CDateTimeContainer(Poco::DateTime & time)
      :m_time(boost::posix_time::from_time_t(Poco::LocalDateTime(time).timestamp().epochTime()))
   {
      
   }

   CDateTimeContainer::~CDateTimeContainer()
   {
   }

   boost::posix_time::ptime CDateTimeContainer::getBoostDateTime() const
   {
      return m_time;
   }

   Poco::DateTime CDateTimeContainer::getPocoDateTime() const
   {
      return Poco::DateTime(Poco::Timestamp::fromEpochTime(boost::posix_time::to_time_t(m_time)));
   }


} } //namespace shared::dateTime