#pragma once
#include "IJob.h"
#include <shared/shared/DataContainer.h>
#include "JobThread.h"
#include "ICondition.h"
#include "IActionList.h"
#include "../communication/ISendMessageAsync.h"

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
      ///\param[in] id        Job ID
      ///\param[in] name      Job name
      ///\param[in] triggers  Trigger condition
      ///\param[in] actions   List of actions
      ///\param[in] pluginGateway plugin access to do actions on plugins
      //-----------------------------------------------------
      CJob(int id, const std::string& name, const shared::CDataContainer& triggers, const shared::CDataContainer& actions, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

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
      boost::shared_ptr<ICondition> m_condition;

      //-----------------------------------------------------
      ///\brief               The actions to do
      //-----------------------------------------------------
      boost::shared_ptr<IActionList> m_actions;
   };
	
} // namespace job	
	
	