#pragma once
#include "yScriptApi/IYScriptApi.h"
#include "ILogger.h"

namespace shared { namespace script
{
   //-----------------------------------------------------
   ///\brief The script runner interface
   //-----------------------------------------------------
   class IRunner
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRunner() {}

      //-----------------------------------------------------
      ///\brief               Request to stop rule
      //-----------------------------------------------------
      virtual void requestStop() = 0;
   };
	
} } // namespace shared::script
	
	