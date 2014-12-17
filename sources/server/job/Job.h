#pragma once
#include "IJob.h"
#include <shared/shared/DataContainer.h>
#include "JobThread.h"
#include "condition/IConditionRoot.h"
#include "condition/IConditionFactory.h"
#include "IActionList.h"
#include "../communication/ISendMessageAsync.h"
#include "../database/sqlite/requesters/Job.h"
#include "INotificationObserverForJobsManager.h"

namespace job
{
   //-----------------------------------------------------
   ///\brief A job
   //-----------------------------------------------------
   class CJob : public IJob, public IJobThreadHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] jobData   Job data (ID, name, conditions, actions...)
      ///\param[in] notificationObserver  The notification observer
      ///\param[in] pluginGateway plugin access to do actions on plugins
      ///\param[in] conditionFactory the condition factory
      //-----------------------------------------------------
      CJob(boost::shared_ptr<database::entities::CJob> jobData,
         boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver,
         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         condition::IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CJob();

      // IJob Implementation
      virtual void start();
      virtual void stop();
      // [END] IJob Implementation

   protected:
      // IJobThreadHandler Implementation
      virtual void doWork();
      // [END] IJobThreadHandler Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Job name
      //-----------------------------------------------------
      const std::string m_name;

      //-----------------------------------------------------
      ///\brief               Job ID
      //-----------------------------------------------------
      const int m_id;

      //-----------------------------------------------------
      ///\brief               Job thread
      //-----------------------------------------------------
      boost::shared_ptr<CJobThread> m_thread;

      //-----------------------------------------------------
      ///\brief               The condition to wait
      //-----------------------------------------------------
      boost::shared_ptr<condition::IConditionRoot> m_condition;

      //-----------------------------------------------------
      ///\brief               The actions to do
      //-----------------------------------------------------
      boost::shared_ptr<IActionList> m_actions;
   };
	
} // namespace job	
	
	