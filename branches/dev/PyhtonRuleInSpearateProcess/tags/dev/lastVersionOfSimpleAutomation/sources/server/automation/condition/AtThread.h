#pragma once
#include <shared/ThreadBase.h>

namespace automation { namespace condition
{      
   //-----------------------------------------------------
   ///\brief The AT operator thread
   //-----------------------------------------------------
   class CAtThread : public shared::CThreadBase
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CAtThread();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAtThread();

      //-----------------------------------------------------
      ///\brief               Start the timer thread
      ///\param[in] timePoint Time point
      ///\param[in] timeoutCallback  Called method when time is out
      //-----------------------------------------------------
      virtual void start(const boost::posix_time::ptime& timePoint, boost::function<void()> timeoutCallback);

      //-----------------------------------------------------
      ///\brief               Main timer thread function
      //-----------------------------------------------------
      virtual void doWork();

   private:
      //-----------------------------------------------------
      ///\brief               Disabled to force to call the public start method
      //-----------------------------------------------------
      virtual void start() { BOOST_ASSERT(false); }

      //-----------------------------------------------------
      ///\brief               The time point
      //-----------------------------------------------------
      boost::posix_time::ptime m_timePoint;

      //-----------------------------------------------------
      ///\brief               Called method when time is out
      //-----------------------------------------------------
      boost::function<void()> m_timeoutCallback;
   };

} } // namespace automation::condition	
	
	