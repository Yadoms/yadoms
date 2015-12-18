#include "stdafx.h"
#include "DateTimeTask.h"
#include <shared/Log.h>
#include "notification/Helpers.hpp"
#include <shared/dateTime/DateTimeContainer.h>

namespace dateTime {

   CDateTimeTask::CDateTimeTask()
   {
   }

   CDateTimeTask::~CDateTimeTask()
   {
   }

   void CDateTimeTask::run()
   {
      boost::posix_time::ptime now = shared::currentTime::Provider::now();
      boost::posix_time::ptime currentTimeWithoutSeconds = boost::posix_time::ptime(now.date(), boost::posix_time::hours(now.time_of_day().hours()) + boost::posix_time::minutes(now.time_of_day().minutes()));

      boost::shared_ptr<shared::dateTime::CDateTimeContainer> notif(new shared::dateTime::CDateTimeContainer(currentTimeWithoutSeconds));
      
      notification::CHelpers::postNotification(notif);
   }

} //namespace dateTime