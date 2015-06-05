#include "stdafx.h"
#include "PluginUpdate.h"
#include <shared/Log.h>
#include "update/worker/Plugin.h"

namespace task { namespace update {

   std::string CPluginUpdate::m_taskName = "plugin.update";

   CPluginUpdate::CPluginUpdate(const std::string & pluginName, const std::string & downloadUrl)
      :m_pluginName(pluginName), m_downloadUrl(downloadUrl)
   {
   }

   CPluginUpdate::~CPluginUpdate()
   {
   }

   const std::string & CPluginUpdate::getName()
   {
      return m_taskName;
   }

   bool CPluginUpdate::doWork(TaskProgressFunc pFunctor)
   {
      YADOMS_LOG(information) << "Start updating plugin...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CPlugin worker(boost::bind(pFunctor, _1, _2, _3, _4));
      worker.update(m_pluginName, m_downloadUrl);

      YADOMS_LOG(information) << "End of updating plugin";
      return true;
   }

} //namespace update
} //namespace task
