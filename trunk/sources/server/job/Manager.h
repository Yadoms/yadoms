#pragma once
#include "IManager.h"
#include "IJob.h"
#include "database/IJobRequester.h"
#include "../communication/ISendMessageAsync.h"

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
      ///\param[in] pluginGateway plugin access to do actions on plugins
      //-----------------------------------------------------
      CManager(boost::shared_ptr<database::IJobRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

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
      ///\brief               The jobs list
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<IJob> > m_jobs;
   };
	
} // namespace job	
	
	