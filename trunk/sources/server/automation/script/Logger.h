#pragma once
#include <shared/script/ILogger.h>
#include <Poco/Logger.h>

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
      //virtual Poco::LogStream& out();
      //virtual Poco::LogStream& error();
      virtual void log(const std::string& msg);
      // [END] ILogger Implementation

   protected:
      ////-----------------------------------------------------
      /////\brief               Returns the date to log
      /////\return              Date to log as string
      ////-----------------------------------------------------
      static std::string now();

   private:
      ////-----------------------------------------------------
      /////\brief               The Logger
      ////-----------------------------------------------------
      //Poco::Logger& m_pocoLogger;

      //-----------------------------------------------------
      ///\brief               The main log file
      //-----------------------------------------------------
      //boost::filesystem::path m_logFile;

      ////-----------------------------------------------------
      /////\brief               The stream associated to logger
      ////-----------------------------------------------------
      //Poco::LogStream m_stream;

      ////-----------------------------------------------------
      /////\brief               The script log file
      ////-----------------------------------------------------
      std::ofstream m_logFile;
   };

} } // namespace automation::script
	
	