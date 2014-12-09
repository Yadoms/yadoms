#pragma once
#include <shared/shared/ThreadBase.h>
#include <server/job/IJobThreadHandler.h>

namespace job
{
   //-----------------------------------------------------
   ///\brief A job
   //-----------------------------------------------------
   class CJobThread : public shared::CThreadBase
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      /// \param [in] jobName The job name, used to name the thread
      //-----------------------------------------------------
      CJobThread(const std::string & jobName, IJobThreadHandler& handler);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CJobThread();

      // CThreadBase implementation
      virtual void doWork();
      // [END] CThreadBase implementation

   private:
      IJobThreadHandler& m_handler;
   };
	
} // namespace job	
	
	