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

   void CPluginRemove::doWork(TaskProgressFunc pFunctor)
   {
      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CPlugin worker(boost::bind(pFunctor, _1, _2, _3, _4, _5));
      worker.remove(m_pluginName);
   }

} //namespace update
} //namespace task
