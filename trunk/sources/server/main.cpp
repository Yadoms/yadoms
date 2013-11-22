/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "Supervisor.h"
#include "StartupOptionsLoader.h"
//TODO #include "SignalHandler.h"


//
// \brief Configure the logger
//
void configureLogger(const IStartupOptions& startupOptions)
{
   boost::log::add_file_log
      (
      boost::log::keywords::file_name = "yadoms_%N.log",                                        
      boost::log::keywords::rotation_size = 10 * 1024 * 1024,                                   
      boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), 
      boost::log::keywords::format = "[%TimeStamp%]: %Message%"                                 
      );

   boost::log::core::get()->set_filter
      (
      boost::log::trivial::severity >= startupOptions.getLogLevel()
      );
}

/*
\brief Main application entry point
*/
int main (int argc, char** argv)
{
   try
   {
      CStartupOptionsLoader startupOptions(argc, argv);

      configureLogger(startupOptions);

      BOOST_LOG_TRIVIAL(info) << "Yadoms is starting";

      CSupervisor supervisor(startupOptions);
      supervisor.start();

      while(1)
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }

      BOOST_LOG_TRIVIAL(info) << "Yadoms is stopped.";
   }
   catch(CStartupOptionsLoaderError& e)
   {
      // Just log to console here
      BOOST_LOG_TRIVIAL(fatal) << e.what();
   }
   catch(...)
   {
      //dual logging in case logger fails/throws
      BOOST_LOG_TRIVIAL(fatal) << "An unhandled exception occurs. Yadoms is now stopped";
      std::cout << "An unhandled exception occurs. Yadoms is now stopped" << std::endl;
   }

   return 0;
}