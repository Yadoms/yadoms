#pragma once
#include <shared/process/ILogger.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The rule logger
   //-----------------------------------------------------
   class CLogger : public shared::process::ILogger
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] pluginName   The plugin name used to identify the logger
      //-----------------------------------------------------
      explicit CLogger(const std::string& pluginName);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLogger();

      // ILogger Implementation
      void log(const std::string& msg) override;
      // [END] ILogger Implementation
   };
} // namespace pluginSystem
