#include "stdafx.h"
#include "UpdateSite.h"
#include <shared/web/UriHelpers.h>

namespace update { namespace info {

   std::string CUpdateSite::m_yadomsLastVersionFilename("lastVersion.json");
   std::string CUpdateSite::m_pluginsLastVersionFilename("package.json");
   std::string CUpdateSite::m_widgetsLastVersionFilename("package.json");
   std::string CUpdateSite::m_distantWindowsFolder("windows");
   std::string CUpdateSite::m_distantLinuxFolder("linux");
   std::string CUpdateSite::m_distantMacOSXFolder("mac");
   std::string CUpdateSite::m_distantRaspberryPIFolder("raspberrypi");
   std::string CUpdateSite::m_distantYadomsBaseFolder("yadoms");
   std::string CUpdateSite::m_distantPluginsBaseFolder("plugins");
   std::string CUpdateSite::m_distantWidgetBaseFolder("widgets");

   
   CUpdateSite::CUpdateSite(boost::shared_ptr<startupOptions::IStartupOptions> & startupOptions, boost::shared_ptr<IRunningInformation> & runningInformation)
      :m_startupOptions(startupOptions), m_currentPlatform(getPlatformFolder(runningInformation))
   {

   }

   CUpdateSite::~CUpdateSite()
   {

   }

   Poco::URI CUpdateSite::getYadomsLastVersionUri()
   {
      Poco::URI result = getYadomsBaseUri();
      shared::web::CUriHelpers::appendPath(result, m_yadomsLastVersionFilename);
      return result;
   }

   Poco::URI CUpdateSite::getYadomsPackageUri(const std::string & packageName)
   {
      Poco::URI result = getYadomsBaseUri();
      shared::web::CUriHelpers::appendPath(result, packageName);
      return result;
   }



   Poco::URI CUpdateSite::getPluginLastVersionUri(const std::string & pluginName)
   {
      Poco::URI result = getPluginBaseUri(pluginName);
      shared::web::CUriHelpers::appendPath(result, m_pluginsLastVersionFilename);
      return result;
   }

   Poco::URI CUpdateSite::getPluginPackageUri(const std::string & pluginName, const std::string & packageName)
   {
      Poco::URI result = getPluginBaseUri(pluginName);
      shared::web::CUriHelpers::appendPath(result, packageName);
      return result;
   }

   Poco::URI CUpdateSite::getWidgetLastVersionUri(const std::string & widgetName)
   {
      Poco::URI result = getWidgetBaseUri(widgetName);
      shared::web::CUriHelpers::appendPath(result, m_widgetsLastVersionFilename);
      return result;
   }

   Poco::URI CUpdateSite::getWidgetPackageUri(const std::string & widgetName, const std::string & packageName)
   {
      Poco::URI result = getWidgetBaseUri(widgetName);
      shared::web::CUriHelpers::appendPath(result, packageName);
      return result;
   }


     

   Poco::URI CUpdateSite::getYadomsBaseUri()
   {
      Poco::URI base(m_startupOptions->getUpdateSiteUri());
      shared::web::CUriHelpers::appendPath(base, m_distantYadomsBaseFolder);
      shared::web::CUriHelpers::appendPath(base, m_currentPlatform);
      return base;
   }

   Poco::URI CUpdateSite::getPluginBaseUri(const std::string & pluginName)
   {
      Poco::URI base(m_startupOptions->getUpdateSiteUri());
      shared::web::CUriHelpers::appendPath(base, m_distantPluginsBaseFolder);
      shared::web::CUriHelpers::appendPath(base, pluginName);
      shared::web::CUriHelpers::appendPath(base, m_currentPlatform);
      return base;
   }

   Poco::URI CUpdateSite::getWidgetBaseUri(const std::string & widgetName)
   {
      Poco::URI base(m_startupOptions->getUpdateSiteUri());
      shared::web::CUriHelpers::appendPath(base, m_distantWidgetBaseFolder);
      shared::web::CUriHelpers::appendPath(base, widgetName);
      return base;
   }


   const std::string CUpdateSite::getPlatformFolder(boost::shared_ptr<IRunningInformation> & runningInformation)
   {
      std::string osName = runningInformation->getOperatingSystemName();

      //determine the main site folder
      std::string platform;
      if (boost::icontains(osName, "win"))
         platform = m_distantWindowsFolder;
      if (boost::icontains(osName, "darwin"))
         platform = m_distantMacOSXFolder;
      if (boost::icontains(osName, "linux"))
      {
         if (boost::icontains(osName, "raspberrypi"))
            platform = m_distantRaspberryPIFolder;
         else
            platform = m_distantLinuxFolder;
      }
      return platform;
   }
} } // namespace update::info