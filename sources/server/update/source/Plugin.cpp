#include "stdafx.h"
#include "Plugin.h"


namespace update {
   namespace source {

      CPlugin::CPlugin()
      {

      }

      CPlugin::~CPlugin()
      {

      }

      const std::string CPlugin::getUpdateLocation() const
      {
         return "plugin";
      }

      const tools::CVersion CPlugin::getInstalledVersion() const
      {
         return tools::CVersion();
      }

      const EUpdateType CPlugin::getUpdateType() const
      {
         return EUpdateType::kPlugin;
      }

   } // namespace source
} // namespace update
