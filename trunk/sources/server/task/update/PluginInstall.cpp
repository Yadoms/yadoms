#include "stdafx.h"
#include "PluginInstall.h"
#include <shared/Log.h>
#include "update/worker/Plugin.h"

namespace task { namespace update {

   std::string CPluginInstall::m_taskName = "plugin.install";

   CPluginInstall::CPluginInstall(const std::string & downloadUrl)
      :m_downloadUrl(downloadUrl)
   {
   }

   CPluginInstall::~CPluginInstall()
   {
   }

   const std::string & CPluginInstall::getName()
   {
      return m_taskName;
   }

   bool CPluginInstall::doWork(TaskProgressFunc pFunctor)
   {

      YADOMS_LOG(information) << "Start installing plugin...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CPlugin::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3);

      ::update::worker::CPlugin worker(adapter);
      worker.install(m_downloadUrl);

      YADOMS_LOG(information) << "End of installing plugin";
      return true;
   }

} //namespace update
} //namespace task
