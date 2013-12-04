#pragma once

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
class CLog
{
public:

   //--------------------------------------------------------------
   /// \brief	                        configure the logger
   /// \param[in]  startupOptions      Startup option of the software
   //--------------------------------------------------------------
   static void configure(const boost::log::trivial::severity_level  & logLevel);

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
   /// \brief     private constructor, this class is static !
   //--------------------------------------------------------------
   CLog();

   //--------------------------------------------------------------
   /// \brief     destructor
   //-------------------------------------------------------------
   virtual ~CLog();
};