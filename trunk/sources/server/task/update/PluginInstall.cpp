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

   void CPluginInstall::doWork(TaskProgressFunc pFunctor)
   {
      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CPlugin worker(boost::bind(pFunctor, _1, _2, _3, _4, _5));
      worker.install(m_downloadUrl);
   }

} //namespace update
} //namespace task
