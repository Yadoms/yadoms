#include "stdafx.h"
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
#include "Log.h"

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

      CLog::configure();
      BOOST_LOG_SCOPED_THREAD_TAG("ThreadName", "Main");

      BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::info) << "Yadoms is starting";
      BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::error) << "test";

      CSupervisor supervisor(startupOptions);
      supervisor.start();

      while(!CApplicationStopHandler::stopRequested())
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }

      supervisor.stop();
      BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::info) << "Yadoms is stopped ";
      //BOOST_LOG_TRIVIAL(info) << "Yadoms is stopped.";
   }
   catch(CStartupOptionsLoaderError& e)
   {
      if (e.isError())
      {
         BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::fatal) << e.what();
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
      BOOST_LOG_SEV(my_logger::get(), boost::log::trivial::fatal) << "An unhandled exception occurs. Yadoms is now stopped";
      std::cout << "An unhandled exception occurs. Yadoms is now stopped" << std::endl;
   }

   return 0;
}