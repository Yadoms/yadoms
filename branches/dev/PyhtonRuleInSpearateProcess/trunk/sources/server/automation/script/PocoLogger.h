#pragma once
#include "ILogger.h"
#include <Poco/Logger.h>

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The rule logger using Poco
   //-----------------------------------------------------
   class CPocoLogger : public ILogger
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] scriptPath The script path where to log in
      //-----------------------------------------------------
      CPocoLogger(const std::string& scriptPath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CPocoLogger();

   public:
      // ILogger Implementation
      virtual void logInformation(const std::string& message);
      virtual void logError(const std::string& message);
      // [END] ILogger Implementation

   private:

      //-----------------------------------------------------
      ///\brief               scriptPath The script path where to log in
      //-----------------------------------------------------
      Poco::Logger& m_pocoLogger;
   };

} } // namespace automation::script
	
	