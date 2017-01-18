#pragma once
#include "logging/YadomsSubModuleProcessLogger.h"

namespace logging
{
   //-----------------------------------------------------
   ///\brief The process logger for script interpreters
   //-----------------------------------------------------
   class CInterpreterLogger : public CYadomsSubModuleProcessLogger//TODO virer ?
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] loggerName   The Logger name
      //-----------------------------------------------------
      explicit CInterpreterLogger(const std::string& loggerName);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CInterpreterLogger();

   protected:
      // CYadomsSubModuleProcessLogger overriding
      std::string extractMessage(const std::string& line,
                                 std::string& outMessage) const override;
      // [END] CYadomsSubModuleProcessLogger overriding
   };
} // namespace logging


