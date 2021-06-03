#pragma once
#include <Poco/Runnable.h>
#include <shared/event/EventHandler.hpp>
#include <shared/versioning/SemVer.h>

#include "automation/IRuleManager.h"
#include "communication/PluginGateway.h"
#include "dataAccessLayer/IDataAccessLayer.h"
#include "database/IDataProvider.h"
#include "dateTime/TimeZoneDatabase.h"
#include "dateTime/TimeZoneProvider.h"
#include "IPathProvider.h"
#include "pluginSystem/Manager.h"
#include "startupOptions/IStartupOptions.h"
#include "update/IUpdateManager.h"
#include "web/IWebServer.h"


//-----------------------------------------------------------------------------
/// \brief              Yadoms supervisor
//-----------------------------------------------------------------------------
class CSupervisor : public Poco::Runnable
{
   //--------------------------------------------------------------
   /// \brief	Event IDs
   //--------------------------------------------------------------
   enum
   {
      kStopRequested = shared::event::kUserFirstId,
      // Yadoms stop was required
   };

public:
   //-----------------------------------------------------------------------------
   /// \brief		                        Constructor
   /// \param[in] pathProvider            The Yadoms paths provider
   /// \param[in] yadomsVersion           The Yadoms version
   //-----------------------------------------------------------------------------
   explicit CSupervisor(boost::shared_ptr<const IPathProvider> pathProvider,
                        const shared::versioning::CSemVer& yadomsVersion);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CSupervisor() = default;

   //-----------------------------------------------------------------------------
   /// \brief		                     The main method (blocking, returns at Yadoms exit)
   //-----------------------------------------------------------------------------
   void run() override;

   //-----------------------------------------------------------------------------
   /// \brief  Ask the supervisor to stop
   //-----------------------------------------------------------------------------
   void requestToStop();

private:
   boost::shared_ptr<web::IWebServer> createPocoBasedWebServer(const boost::shared_ptr<const startupOptions::IStartupOptions>& startupOptions,
                                                               const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
                                                               boost::shared_ptr<database::IDataProvider> dataProvider,
                                                               boost::shared_ptr<pluginSystem::CManager> pluginManager,
                                                               const boost::shared_ptr<communication::CPluginGateway>& pluginGateway,
                                                               boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
                                                               boost::shared_ptr<automation::IRuleManager> automationRulesManager,
                                                               boost::shared_ptr<update::IUpdateManager> updateManager,
                                                               boost::shared_ptr<task::CScheduler> taskManager) const;
   boost::shared_ptr<web::IWebServer> createOatppBasedWebServer(const boost::shared_ptr<const startupOptions::IStartupOptions>& startupOptions,
                                                                const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
                                                                boost::shared_ptr<database::IDataProvider> dataProvider,
                                                                boost::shared_ptr<pluginSystem::CManager> pluginManager,
                                                                const boost::shared_ptr<communication::CPluginGateway>& pluginGateway,
                                                                boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
                                                                boost::shared_ptr<automation::IRuleManager> automationRulesManager,
                                                                boost::shared_ptr<update::IUpdateManager> updateManager,
                                                                boost::shared_ptr<task::CScheduler> taskManager) const;


   //-----------------------------------------------------------------------------
   /// \brief		                     The supervisor event handler
   //-----------------------------------------------------------------------------
   shared::event::CEventHandler m_eventHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     The Yadoms paths provider
   //-----------------------------------------------------------------------------
   boost::shared_ptr<const IPathProvider> m_pathProvider;

   const shared::versioning::CSemVer m_yadomsVersion;
};
