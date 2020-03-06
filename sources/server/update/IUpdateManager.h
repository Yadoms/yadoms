#pragma once
#include <shared/DataContainer.h>

namespace update
{
   //-----------------------------------------------------------------------------
   /// \brief  Provide methods for starting update and check for updates
   //-----------------------------------------------------------------------------   
   class IUpdateManager
   {
   public:
      virtual ~IUpdateManager()
      {
      }
      
      virtual std::string scanForUpdatesAsync() = 0;
      virtual shared::CDataContainerSharedPtr getUpdates(bool includePreleases) const = 0;

      virtual std::string updatePluginAsync(const std::string& pluginName, const std::string& downloadUrl) const = 0;
      virtual std::string installPluginAsync(const std::string& downloadUrl) const = 0;
      virtual std::string removePluginAsync(const std::string& pluginName) const = 0;
      virtual std::string updateWidgetAsync(const std::string& widgetName, const std::string& downloadUrl) const = 0;
      virtual std::string installWidgetAsync(const std::string& downloadUrl) const = 0;
      virtual std::string removeWidgetAsync(const std::string& widgetName) const = 0;
      virtual std::string updateScriptInterpreterAsync(const std::string& scriptInterpreterName, const std::string& downloadUrl) const = 0;
      virtual std::string installScriptInterpreterAsync(const std::string& downloadUrl) const = 0;
      virtual std::string removeScriptInterpreterAsync(const std::string& scriptInterpreterName) const = 0;
      virtual std::string updateYadomsAsync(const std::string& downloadUrl) const = 0;
   };
} // namespace update
