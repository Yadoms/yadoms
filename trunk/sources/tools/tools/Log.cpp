#include "stdafx.h"
#include "log.h"

CLog::CLog()
{
}

CLog::~CLog()
{
}

void CLog::configure(const boost::log::trivial::severity_level  & logLevel)
{
   //add console output (one for all)
   CreateConsoleSink();

   //add log per thread
   CreateFilePerThreadSink();
   
   // Add some attributes too
   boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
   boost::log::core::get()->add_global_attribute("RecordID", boost::log::attributes::counter< unsigned int >());

   //we filter on severity
   boost::log::core::get()->set_filter(boost::log::trivial::severity >= logLevel);

}


void CLog::CreateFilePerThreadSink()
{
   ///////////////////////////////////////////////////////////////////////
   // Add log file output (one file per thread)
   ///////////////////////////////////////////////////////////////////////

   // Create a text file sink
   typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_multifile_backend > file_sink;
   boost::shared_ptr< file_sink > sink(new file_sink);

   // Set up the file per thread naming   :  logs/threadname.log
   sink->locked_backend()->set_file_name_composer(boost::log::sinks::file::as_file_name_composer(boost::log::expressions::stream << "logs/" << boost::log::expressions::attr< std::string >("ThreadName") << ".log"));

   // Set up the log record formatter
   //TimeStamp [ThreadName] Severity : message
   sink->set_formatter
      (
      boost::log::expressions::format("%1% [%2%] %3% - %4%")
      % boost::log::expressions::attr< boost::posix_time::ptime >("TimeStamp")
      % boost::log::expressions::attr< std::string >("ThreadName")
      % boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity")
      % boost::log::expressions::smessage
      );

   // Add it to the core
   boost::log::core::get()->add_sink(sink);
}

void CLog::CreateConsoleSink()
{
   // Create a backend and attach console log stream
   boost::shared_ptr< boost::log::sinks::text_ostream_backend > backend = boost::make_shared< boost::log::sinks::text_ostream_backend >();
   backend->add_stream(boost::shared_ptr< std::ostream >(&std::clog));
   // Enable auto-flushing after each log record written
   backend->auto_flush(true);

   // Wrap it into the frontend and register in the core.
   // The backend requires synchronization in the frontend.
   typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > sink_t;
   boost::shared_ptr< sink_t > sinklog(new sink_t(backend));
   
   sinklog->set_formatter
      (
      boost::log::expressions::format("%1% [%2%] %3% - %4%")
      % boost::log::expressions::attr< boost::posix_time::ptime >("TimeStamp")
      % boost::log::expressions::attr< std::string >("ThreadName")
      % boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity")
      % boost::log::expressions::smessage
      );

   boost::log::core::get()->add_sink(sinklog);
}
