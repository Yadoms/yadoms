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

   void CPluginUpdate::doWork(TaskProgressFunc pFunctor)
   {
      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CPlugin worker(boost::bind(pFunctor, _1, _2, _3, _4, _5));
      worker.update(m_pluginName, m_downloadUrl);
   }

} //namespace update
} //namespace task
