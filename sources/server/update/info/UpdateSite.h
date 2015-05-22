#pragma once

#include "startupOptions/IStartupOptions.h"
#include "IRunningInformation.h"
#include <Poco/URI.h>
#include <shared/DataContainer.h>

namespace update { namespace info {

   //---------------------------------------------
   ///\brief   Provide information about update site
   //---------------------------------------------
   class CUpdateSite
   {
   public:
      //---------------------------------------------
      ///\brief   Constructor
      ///\param [in] startupOptions The startup options
      ///\param [in] runningInformation The running information provider
      //---------------------------------------------
      CUpdateSite(boost::shared_ptr<startupOptions::IStartupOptions> & startupOptions, boost::shared_ptr<IRunningInformation> & runningInformation);

      //---------------------------------------------
      ///\brief   Destructor
      //---------------------------------------------
      virtual ~CUpdateSite();

      //---------------------------------------------
      ///\brief   Give the Yadoms lastVersion.json URI
      ///\return the Yadoms lastVersion.json URI : 
      /// i.e. : http://www.yadoms.com/downloads/update/yadoms/windows/lastVersion.json
      //---------------------------------------------
      Poco::URI getYadomsLastVersionUri();

      //---------------------------------------------
      ///\brief   Give the Yadoms package URI
      ///\param [in] packageName The package name (ie: package2.0.0.0.zip)
      ///\return the Yadoms package URI
      /// i.e. : http://www.yadoms.com/downloads/update/yadoms/windows/package2.0.0.0.zip
      //---------------------------------------------
      Poco::URI getYadomsPackageUri(const std::string & packageName);


      //---------------------------------------------
      ///\brief   Give a plugin package.json URI
      ///\param [in] pluginName The plugin name (ie: fakePlugin)
      ///\return the plugin package.json URI : 
      /// i.e. : http://www.yadoms.com/downloads/update/plugins/fakePlugin/windows/package.json
      //---------------------------------------------
      Poco::URI getPluginLastVersionUri(const std::string & pluginName);

      //---------------------------------------------
      ///\brief   Give a plugin package URI
      ///\param [in] pluginName The plugin name (ie: fakePlugin)
      ///\param [in] packageName The package name (ie: package2.0.0.0.zip)
      ///\return the plugin package URI
      /// i.e. : http://www.yadoms.com/downloads/update/plugins/fakePlugin/windows/package2.0.0.0.zip
      //---------------------------------------------
      Poco::URI getPluginPackageUri(const std::string & pluginName, const std::string & packageName);    
      
      
      //---------------------------------------------
      ///\brief   Give a widget package.json URI
      ///\param [in] widgetName The widget name (ie: meteo)
      ///\return the widget package.json URI : 
      /// i.e. : http://www.yadoms.com/downloads/update/widgets/meteo/package.json
      //---------------------------------------------
      Poco::URI getWidgetLastVersionUri(const std::string & widgetName);

      //---------------------------------------------
      ///\brief   Give a widget package URI
      ///\param [in] widgetName The widget name (ie: meteo)
      ///\param [in] packageName The package name (ie: package2.0.0.0.zip)
      ///\return the widget package URI
      /// i.e. : http://www.yadoms.com/downloads/update/widgets/meteo/package2.0.0.0.zip
      //---------------------------------------------
      Poco::URI getWidgetPackageUri(const std::string & widgetName, const std::string & packageName);



      //---------------------------------------------
      ///\brief   Get all the available versions for a plugin
      ///\param [in] displayLanguage The language used for display
      ///\return the list of plugin version
      //---------------------------------------------
      shared::CDataContainer getAllPluginVersions(const std::string & displayLanguage);

   private:
      //---------------------------------------------
      ///\brief   Give the Yadoms URI base for current platform
      ///\return the Yadoms URI base for current platform
      /// i.e. : http://www.yadoms.com/downloads/update/yadoms/windows
      //---------------------------------------------
      Poco::URI getYadomsBaseUri();   
      
      //---------------------------------------------
      ///\brief   Give the plugin URI base for current platform
      ///\param [in] pluginName The plugin name (ie: fakePlugin)
      ///\return the Yadoms URI base for current platform
      /// i.e. : http://www.yadoms.com/downloads/update/plugins/fakePlugin/windows
      //---------------------------------------------
      Poco::URI getPluginBaseUri(const std::string & pluginName);    
      
      //---------------------------------------------
      ///\brief   Give the widget URI base (common for all platforms)
      ///\param [in] widgetName The widget name (ie: meteo)
      ///\return the widget URI base (common for all platforms)
      /// i.e. : http://www.yadoms.com/downloads/update/widget/meteo
      //---------------------------------------------
      Poco::URI getWidgetBaseUri(const std::string & widgetName);

      //---------------------------------------------
      ///\brief   Give the platform folder
      ///\param [in] runningInformation The running information (provide the running platform)
      ///\return the platform distant folder
      /// i.e. : windows, linux, mac, raspberry,...
      //---------------------------------------------
      const std::string getPlatformFolder(boost::shared_ptr<IRunningInformation> & runningInformation);

      //---------------------------------------------
      ///\brief   The startup options
      //---------------------------------------------
      boost::shared_ptr<startupOptions::IStartupOptions> m_startupOptions;

      //---------------------------------------------
      ///\brief   The current platform
      //---------------------------------------------
      std::string m_currentPlatform;


   private:

      //---------------------------------------------
      ///\brief   The yadoms lastVersion filename
      //---------------------------------------------
      static std::string m_yadomsLastVersionFilename;  
      
      //---------------------------------------------
      ///\brief   The plugins lastVersion filename
      //---------------------------------------------
      static std::string m_pluginsLastVersionFilename;  
      
      //---------------------------------------------
      ///\brief   The widgets lastVersion filename
      //---------------------------------------------
      static std::string m_widgetsLastVersionFilename;

      //---------------------------------------------
      ///\brief   The distant folder name for yadoms
      //---------------------------------------------
      static std::string m_distantYadomsBaseFolder;
      //---------------------------------------------
      ///\brief   The distant folder name for plugins
      //---------------------------------------------
      static std::string m_distantPluginsBaseFolder;
      //---------------------------------------------
      ///\brief   The distant folder name for widgets
      //---------------------------------------------
      static std::string m_distantWidgetBaseFolder;

      //---------------------------------------------
      ///\brief   The distant folder name for Windows platform 
      //---------------------------------------------
      static std::string m_distantWindowsFolder;
      //---------------------------------------------
      ///\brief   The distant folder name for Linux platform 
      //---------------------------------------------
      static std::string m_distantLinuxFolder;
      //---------------------------------------------
      ///\brief   The distant folder name for MacOSX platform 
      //---------------------------------------------
      static std::string m_distantMacOSXFolder;
      //---------------------------------------------
      ///\brief   The distant folder name for RaspberryPI platform 
      //---------------------------------------------
      static std::string m_distantRaspberryPIFolder;
   };

   
} } // namespace update::info
