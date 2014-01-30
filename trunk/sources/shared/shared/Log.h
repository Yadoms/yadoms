#pragma once

#include <shared/Export.h>

typedef boost::log::sources::severity_channel_logger_mt<
    boost::log::trivial::severity_level,     // the type of the severity level
    std::string         // the type of the channel name
> my_logger_mt;

// Global logger declaration
BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(my_logger, my_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return my_logger_mt(boost::log::keywords::channel = "my_logger");
}

//--------------------------------------------------------------
/// \brief     Macro for easy logging:
/// YADOMS_LOG(error) << "this is an error";
//-------------------------------------------------------------
#define YADOMS_LOG(lvl) BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::lvl)

//--------------------------------------------------------------
/// \brief     Macro for easy configure the log:
/// YADOMS_LOG_CONFIGURE("Main");  //create a log file named main.log for the thread which calls this macro
//-------------------------------------------------------------
#define YADOMS_LOG_CONFIGURE(threadname) BOOST_LOG_SCOPED_THREAD_TAG("ThreadName", threadname)


//--------------------------------------------------------------
/// \brief     Class used to configure the logger
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CLog
{
public:

   //--------------------------------------------------------------
   /// \brief	                  configure the logger with one file per thread without rolling on size
   /// \param[in]  logLevel      The log level
   //--------------------------------------------------------------
   static void configure_file_per_thread(const boost::log::trivial::severity_level  & logLevel);

   //--------------------------------------------------------------
   /// \brief	                  configure the logger with only one file with a rolling file size
   /// \param[in]  logLevel      The log level
   //--------------------------------------------------------------
   static void configure_one_rolling_file(const boost::log::trivial::severity_level  & logLevel);
private:

   //--------------------------------------------------------------
   /// \brief     configure the logger with a file per thread
   //--------------------------------------------------------------
   static void CreateFilePerThreadSink();

   //--------------------------------------------------------------
   /// \brief     configure the logger for console logging
   //--------------------------------------------------------------
   static void CreateConsoleSink();

   //--------------------------------------------------------------
   /// \brief     configure the logger for logging into rolling file
   //--------------------------------------------------------------
   static void CreateRollingFileSink();


   //--------------------------------------------------------------
   /// \brief     private constructor, this class is static !
   //--------------------------------------------------------------
   CLog();

   //--------------------------------------------------------------
   /// \brief     destructor
   //-------------------------------------------------------------
   virtual ~CLog();
};