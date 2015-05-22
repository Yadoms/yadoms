#include "stdafx.h"
#include "Plugin.h"


namespace update {
   namespace source {

      CPlugin::CPlugin(const std::string & pluginName, const tools::CVersion & installedVersion)
         :m_pluginName(pluginName), m_installedVersion(installedVersion)
      {

      }

      CPlugin::~CPlugin()
      {

      }

      const EUpdateType CPlugin::getUpdateType() const
      {
         return EUpdateType::kPlugin;
      }

      const std::string & CPlugin::getPluginName() const
      {
         return m_pluginName;
      }

      const tools::CVersion & CPlugin::getInstalledVersion() const
      {
         return m_installedVersion;
      }


   } // namespace source
} // namespace update
