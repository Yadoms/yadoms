#pragma once
#include "IUpdateSource.h"

namespace update {
   namespace source {

      class CPlugin : public IUpdateSource
      {
      public:
         //---------------------------------------------
         ///\brief   Constructor
         //---------------------------------------------
         CPlugin(const std::string & pluginName, const tools::CVersion & installedVersion);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CPlugin();

         // IUpdateSource implementation 
         virtual const EUpdateType getUpdateType() const;
         // [END] - IUpdateSource implementation 

         const std::string & getPluginName() const;
         const tools::CVersion & getInstalledVersion() const;

      private:
         std::string m_pluginName;
         tools::CVersion m_installedVersion;
      };

   } // namespace source
} // namespace update
