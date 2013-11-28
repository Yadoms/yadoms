/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "Supervisor.h"
#include "StartupOptionsLoader.h"
#include "ApplicationStopHandler.h"

// Global logger declaration
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, boost::log::sources::logger_mt)

   //
   // \brief Configure the logger
   //
   void configureLogger(const IStartupOptions& startupOptions)
{
   // Create a text file sink
   typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_multifile_backend > file_sink;
   boost::shared_ptr< file_sink > sink(new file_sink);

   // Set up how the file names will be generated
   sink->locked_backend()->set_file_name_composer(boost::log::sinks::file::as_file_name_composer(
      boost::log::expressions::stream << "logs/" << boost::log::expressions::attr< boost::thread::id >("ThreadID") << ".log"));

   // Set the log record formatter
   sink->set_formatter
      (
      boost::log::expressions::format("%1%: [%2%] - %3%")
      % boost::log::expressions::attr< unsigned int >("RecordID")
      % boost::log::expressions::attr< boost::posix_time::ptime >("TimeStamp")
      % boost::log::expressions::smessage
      );

   // Add it to the core
   boost::log::core::get()->add_sink(sink);

   // Add some attributes too
   boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
   boost::log::core::get()->add_global_attribute("RecordID", boost::log::attributes::counter< unsigned int >());
}

void stopHandler()
{
   //TODO
}

/*
\brief Main application entry point
*/
int main (int argc, char** argv)
{
   CApplicationStopHandler::configure();

   try
   {
      CStartupOptionsLoader startupOptions(argc, argv);

      configureLogger(startupOptions);
      BOOST_LOG_SCOPED_THREAD_TAG("ThreadID", boost::this_thread::get_id());
      BOOST_LOG(my_logger::get()) << "Yadoms is starting";

      //BOOST_LOG_TRIVIAL(info) << "Yadoms is starting";

      CSupervisor supervisor(startupOptions);
      supervisor.start();

      while(!CApplicationStopHandler::stopRequested())
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }

      supervisor.stop();
      BOOST_LOG(my_logger::get()) << "Yadoms is stopped ";
      //BOOST_LOG_TRIVIAL(info) << "Yadoms is stopped.";
   }
   catch(CStartupOptionsLoaderError& e)
   {
      if (e.isError())
      {
         BOOST_LOG_TRIVIAL(fatal) << e.what();
      }
      else
      {
         // Help was invoked, just print to console
         std::cout << e.what() << std::endl;
      }
   }
   catch(...)
   {
      //dual logging in case logger fails/throws
      BOOST_LOG_TRIVIAL(fatal) << "An unhandled exception occurs. Yadoms is now stopped";
      std::cout << "An unhandled exception occurs. Yadoms is now stopped" << std::endl;
   }

   return 0;
}