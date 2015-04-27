#include "stdafx.h"
#include "Plugin.h"


namespace update {
   namespace worker {

      CPlugin::CPlugin(boost::shared_ptr<update::source::CPlugin> source)
         :m_source(source)
      {

      }

      CPlugin::~CPlugin()
      {

      }

      void CPlugin::checkForUpdateAsync(WorkerProgressFunc callback)
      {

      }

      void CPlugin::updateAsync(WorkerProgressFunc callback)
      {

      }

   } // namespace worker
} // namespace update
