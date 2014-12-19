#pragma once
#include "IManager.h"
#include "IJob.h"
#include "condition/IConditionFactory.h"
#include "database/IJobRequester.h"
#include "../communication/ISendMessageAsync.h"
#include <shared/notification/NotificationCenter.h>
#include "INotificationObserverForJobsManager.h"
#include "database/IAcquisitionRequester.h"

namespace job
{
   //-----------------------------------------------------
   ///\brief The jobs manager
   //-----------------------------------------------------
   class CManager : public IManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] dbRequester  Database requester
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      //-----------------------------------------------------
      CManager(boost::shared_ptr<database::IJobRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CManager();

      // IManager Implementation
      virtual void start();
      virtual void stop();
      // [END] IManager Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The plugin access (to send commands to plugins)
      //-----------------------------------------------------
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;

      //-----------------------------------------------------
      ///\brief               The job data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IJobRequester> m_dbRequester;

      //-----------------------------------------------------
      ///\brief               The condition factory
      //-----------------------------------------------------
      boost::shared_ptr<condition::IConditionFactory> m_conditionFactory;

      //-----------------------------------------------------
      ///\brief               The notification center
      //-----------------------------------------------------
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;

      //-----------------------------------------------------
      ///\brief               The notification observer
      //-----------------------------------------------------
      boost::shared_ptr<INotificationObserverForJobsManager> m_notificationObserver;

      //-----------------------------------------------------
      ///\brief               The jobs list
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<IJob> > m_jobs;
   };
	
} // namespace job	
	
	