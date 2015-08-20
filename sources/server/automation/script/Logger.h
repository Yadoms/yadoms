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
      CLogger(const boost::filesystem::path& scriptPath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CLogger();

      //-----------------------------------------------------
      ///\brief               Get the full path of the script log file 
      ///\return              Full path of the script log file
      //-----------------------------------------------------
      static boost::filesystem::path logFile(const boost::filesystem::path& scriptPath);

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
      ///\brief               The main log file
      //-----------------------------------------------------
      boost::filesystem::path m_logFile;

      //-----------------------------------------------------
      ///\brief               The stream associated to logger
      //-----------------------------------------------------
      Poco::LogStream m_stream;
   };

} } // namespace automation::script
	
	