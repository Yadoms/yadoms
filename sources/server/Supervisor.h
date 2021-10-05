#pragma once
#include <Poco/Runnable.h>
#include <shared/event/EventHandler.hpp>
#include <shared/versioning/SemVer.h>
#include "IPathProvider.h"
#include "dataAccessLayer/IDataAccessLayer.h"
#include "dateTime/TimeZoneDatabase.h"
#include "dateTime/TimeZoneProvider.h"
#include "web/IWebServer.h"
#include "web/rest/service/IRestService.h"


//-----------------------------------------------------------------------------
/// \brief              Yadoms supervisor
//-----------------------------------------------------------------------------
class CSupervisor final : public Poco::Runnable
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
   CSupervisor(boost::shared_ptr<const IPathProvider> pathProvider,
               const shared::versioning::CSemVer& yadomsVersion);

   ~CSupervisor() override = default;

   //-----------------------------------------------------------------------------
   /// \brief		                     The main method (blocking, returns at Yadoms exit)
   //-----------------------------------------------------------------------------
   void run() override;

   //-----------------------------------------------------------------------------
   /// \brief  Ask the supervisor to stop
   //-----------------------------------------------------------------------------
   void requestToStop();

private:
   boost::shared_ptr<web::IWebServer> createPocoBasedWebServer(
      const std::string& address,
      unsigned short port,
      bool useSsl,
      unsigned short securedPort,
      bool allowExternalAccess,
      const boost::filesystem::path& webServerPath,
      const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
      const boost::shared_ptr<std::vector<boost::shared_ptr<web::rest::service::IRestService>>>& restServices,
      const boost::shared_ptr<std::map<std::string, boost::filesystem::path>>& aliases,
      const boost::shared_ptr<dataAccessLayer::IConfigurationManager>& configurationManager,
      bool skipPasswordCheck) const;

   boost::shared_ptr<web::IWebServer> createOatppBasedWebServer(
      const std::string& address,
      unsigned short port,
      bool useSsl,
      unsigned short securedPort,
      bool allowExternalAccess,
      const boost::filesystem::path& webServerPath,
      const boost::shared_ptr<dataAccessLayer::IDataAccessLayer>& dataAccessLayer,
      boost::shared_ptr<std::vector<boost::shared_ptr<web::rest::service::IRestService>>> restServices,
      boost::shared_ptr<std::map<std::string, boost::filesystem::path>> aliases,
      const boost::shared_ptr<dataAccessLayer::IConfigurationManager>& configurationManager,
      bool skipPasswordCheck) const;


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
