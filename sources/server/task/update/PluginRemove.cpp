#include "stdafx.h"
#include "PluginRemove.h"
#include <shared/Log.h>

#include "update/worker/Plugin.h"

namespace task { namespace update {

   std::string CPluginRemove::m_taskName = "plugin.remove";

   CPluginRemove::CPluginRemove(const std::string & pluginName)
      :m_pluginName(pluginName)
   {
   }

   CPluginRemove::~CPluginRemove()
   {
   }

   const std::string & CPluginRemove::getName()
   {
      return m_taskName;
   }

   bool CPluginRemove::doWork(TaskProgressFunc pFunctor)
   {
      YADOMS_LOG(information) << "Start removing plugin...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CPlugin::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3);

      ::update::worker::CPlugin worker(adapter);
      worker.remove(m_pluginName);

      YADOMS_LOG(information) << "End of removing plugin";
      return true;
   }

} //namespace update
} //namespace task
