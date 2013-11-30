#include "stdafx.h"
/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/
#include "Supervisor.h"
#include "StartupOptionsLoader.h"
#include "ApplicationStopHandler.h"


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

      //configureLogger(startupOptions);

      BOOST_LOG_TRIVIAL(info) << "Yadoms is starting";

      CSupervisor supervisor(startupOptions);
      supervisor.start();

      while(!CApplicationStopHandler::stopRequested())
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }

      supervisor.stop();

      BOOST_LOG_TRIVIAL(info) << "Yadoms is stopped.";
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