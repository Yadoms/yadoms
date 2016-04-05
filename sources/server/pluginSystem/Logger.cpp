#include "stdafx.h"
#include "Logger.h"
#include <shared/Log.h>


namespace pluginSystem
{
   CLogger::CLogger(const std::string& pluginName)
   {
      YADOMS_LOG_CONFIGURE("plugin/" + pluginName);
   }

   CLogger::~CLogger()
   {
   }

   void CLogger::log(const std::string& msg)
   {
      // TODO gérer les niveaux le log ?
      YADOMS_LOG(information) << msg;
   }
} // namespace pluginSystem

