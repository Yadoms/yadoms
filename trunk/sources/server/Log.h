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


class CLog
{
public:

   //--------------------------------------------------------------
   /// \brief	                        configure the logger
   /// \param[in]  startupOptions      Startup option of the software
   //--------------------------------------------------------------
   static void configure(const IStartupOptions& startupOptions)
   {
      // Create a text file sink
      typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_multifile_backend > file_sink;
      boost::shared_ptr< file_sink > sink(new file_sink);

      // Set up how the file names will be generated
      boost::phoenix::expression::argument< 2 >::type astream;
      astream << "logs/" ;
      astream << boost::log::expressions::attr< std::string >("ThreadName") ;
      astream << ".log";
      sink->locked_backend()->set_file_name_composer(boost::log::sinks::file::as_file_name_composer(astream));

      // Set the log record formatter
      boost::phoenix::expression::argument< 2 >::type aformatter;
      aformatter << boost::log::expressions::attr< boost::posix_time::ptime >("TimeStamp");
      aformatter << " [";
      aformatter << boost::log::expressions::attr< std::string >("ThreadName");
      aformatter << "] ";
      aformatter << boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity");
      aformatter << " ";
      aformatter << boost::log::expressions::smessage;
      sink->set_formatter(aformatter);
         

      // Add it to the core
      boost::log::core::get()->add_sink(sink);

      // Add some attributes too
      boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
      boost::log::core::get()->add_global_attribute("RecordID", boost::log::attributes::counter< unsigned int >());

      
      //todo : line do not compiles with trivial sample
      //boost::log::core::get()->set_filter(...);
   
   }

private:
   CLog()
   {
   }

   virtual ~CLog(void)
   {
   }
};