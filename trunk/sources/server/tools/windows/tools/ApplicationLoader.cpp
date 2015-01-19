#include "stdafx.h"
#include "ApplicationLoader.h"
#include "service/ApplicationService.h"
#include <shared/Log.h>
#include <server/startupOptions/LoaderException.hpp>

namespace tools {

   int CApplicationLoader::run(IApplication & application, service::IApplicationServiceInformation & info, int argc, char ** argv)
   {
      //configure application with command line parameters
      try
      {
         application.configure(argc, argv);
      }
      catch (startupOptions::CLoaderException& e)
      {
         if (e.isError())
         {
            YADOMS_LOG(fatal) << e.what();
            return 1;
         }

         // Help was invoked, just print to console (independent to log level, so not use log)
         std::cout << e.what();
         return 0;
      }      

      //find the working dir (= path to exe)
      boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
      full_path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));

      service::CApplicationService service(application, info.getServiceName(), full_path.parent_path());
      if (!service::CServiceBase::Run(service))
      {
         if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
         {
            YADOMS_LOG(information) << "Running Yadoms as console application";
            //running from console, just start application without service behavior
            return application.run();
         }
         
         YADOMS_LOG(fatal) << boost::format("Service failed to run w / err 0x%08X") % ERROR_FAILED_SERVICE_CONTROLLER_CONNECT;
      }

      return 0;
   }

} //namespace tools
