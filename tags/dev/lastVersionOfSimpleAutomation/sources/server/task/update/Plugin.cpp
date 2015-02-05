#include "stdafx.h"

#include <shared/Log.h>
#include "task/ITask.h"

#include "Plugin.h"
namespace task { namespace update {

   std::string CPlugin::m_taskName = "system.pluginUpdate";

   CPlugin::CPlugin()
   {
   }

   CPlugin::~CPlugin()
   {
   }

   const std::string & CPlugin::getName()
   {
      return m_taskName;
   }

   bool CPlugin::doWork(TaskProgressFunc pFunctor)
   {

      YADOMS_LOG(information) << "Start updating plugin...";

      for(int i=1; i<=10; ++i)
      {
         boost::this_thread::sleep(boost::posix_time::seconds(1)); 
         pFunctor(i*10.0f, "");
      }

      YADOMS_LOG(information) << "End of updating plugin";
      return true;
   }

} //namespace update
} //namespace task
