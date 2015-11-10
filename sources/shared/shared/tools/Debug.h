#pragma once
#include <shared/Export.h>

namespace shared { namespace tools {

   //---------------------------------------------
   ///\brief Class which provide utilities for debug
   //---------------------------------------------
   class YADOMS_SHARED_EXPORT CDebug
   {
   public:
      //---------------------------------------------
      ///\brief Generate a random number between min and max (included)
      ///\param [in] min  The minimum value (included)
      //---------------------------------------------
      static void LogThreadId(const std::string & threadName);

      static void CreateThreadMonitoring();
   };

} // namespace tools 
} // namespace shared