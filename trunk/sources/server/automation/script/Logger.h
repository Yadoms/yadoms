#pragma once
#include <shared/script/ILogger.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The rule logger
   //-----------------------------------------------------
   class CLogger : public shared::script::ILogger
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] scriptPath The script path where to log in
      //-----------------------------------------------------
      CLogger(const std::string& scriptPath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLogger();

   public:
      // ILogger Implementation
      virtual std::ostream& out();
      virtual std::ostream& error();
      // [END] ILogger Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The Logger
      //-----------------------------------------------------
      Poco::Logger& m_pocoLogger;

      //-----------------------------------------------------
      ///\brief               The stream associated to logger
      //-----------------------------------------------------
      Poco::LogStream m_stream;
   };

} } // namespace automation::script
	
	