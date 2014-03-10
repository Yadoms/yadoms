#include "stdafx.h"
#include "Plugin.h"
#include <shared/Log.h>

namespace task { namespace update {

   std::string CPlugin::m_taskName = "update plugin task";

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

   void CPlugin::doWork(TaskProgressFunc pFunctor)
   {

      YADOMS_LOG(info) << "Start updating plugin...";

      for(int i=1; i<=10; ++i)
      {
         boost::this_thread::sleep(boost::posix_time::seconds(1)); 
         pFunctor(i*10.0f);
      }

      YADOMS_LOG(info) << "End of updating plugin";
   }

} //namespace update
} //namespace task
