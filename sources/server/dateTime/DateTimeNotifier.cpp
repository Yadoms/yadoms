#include "stdafx.h"
#include "DateTimeNotifier.h"
#include <shared/Log.h>
#include "DateTimeTask.h"
#include <Poco/DateTime.h>
#include "notification/Helpers.hpp"


namespace dateTime {
   
   CDateTimeNotifier::CDateTimeNotifier()
   {
      //create the summary computing task
      m_dateTimeTask = Poco::Util::TimerTask::Ptr(new CDateTimeTask());
   }

   CDateTimeNotifier::~CDateTimeNotifier()
   {
      //stop();
   }
   
   void CDateTimeNotifier::start()
   {
      //then schedule it to be run each hour (+1 minute in order to give a small latency for plugins): 00h01, 01h01, 02h01, 03h01, 04h01....
      //working with Poco::DateTime because using Poco::Util::Timer class
      Poco::DateTime now;
      Poco::DateTime firstOccurence(now.year(), now.month(), now.day(), now.hour(), now.minute());
      Poco::Timespan oneMinuteOffset(0, 0, 1, 0, 0);
      firstOccurence += oneMinuteOffset;//+1minute
      Poco::Timespan timeToWaitBeforeFirstOccurrence = firstOccurence - now;

      //schedule the timer to launch task each hour +1min
      //Schedule : now and each hour (1000 * 3600)
      long msWait = (long)timeToWaitBeforeFirstOccurrence.totalMilliseconds() + 200; //ad 200ms to ensure system time minute is changed, without it sometime yadoms reacts faster than system time. force cast because value is maximum 1hour = 1000*3600 which is less than "long" maximum value
      long msWaitPeriod = (long)oneMinuteOffset.totalMilliseconds();//force cast because value is 1 hour = 1000*3600 which is less than "long" maximum value

      m_dateTimeTimer.scheduleAtFixedRate(m_dateTimeTask, msWait, msWaitPeriod);
   }

   void CDateTimeNotifier::stop()
   {
      m_dateTimeTimer.cancel(true);
      if(m_dateTimeTask)
         m_dateTimeTask->cancel();
   }
} //namespace dateTime