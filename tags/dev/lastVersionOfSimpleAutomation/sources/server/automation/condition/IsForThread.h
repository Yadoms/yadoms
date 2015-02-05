#pragma once
#include <shared/ThreadBase.h>

namespace automation { namespace condition
{      
   //-----------------------------------------------------
   ///\brief The IS...FOR operator thread
   //-----------------------------------------------------
   class CIsForThread : public shared::CThreadBase
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CIsForThread();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CIsForThread();

      //-----------------------------------------------------
      ///\brief               Start the timer thread
      ///\param[in] duration  Timer duration
      ///\param[in] timeoutCallback  Called method when time is out
      //-----------------------------------------------------
      virtual void start(const boost::posix_time::time_duration& duration, boost::function<void()> timeoutCallback);

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
      ///\brief               Timer duration
      //-----------------------------------------------------
      boost::posix_time::time_duration m_duration;

      //-----------------------------------------------------
      ///\brief               Called method when time is out
      //-----------------------------------------------------
      boost::function<void()> m_timeoutCallback;
   };

} } // namespace automation::condition	
	
	