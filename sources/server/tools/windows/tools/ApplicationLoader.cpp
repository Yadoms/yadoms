#include "stdafx.h"
#include "ApplicationLoader.h"
#include "service/ApplicationService.h"
#include <shared/Log.h>

namespace tools {

   int CApplicationLoader::run(IApplication & application, service::IApplicationServiceInformation & info, int argc, char ** argv)
   {
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
         else
         {
            YADOMS_LOG(fatal) << boost::format("Service failed to run w / err 0x%08X") % ERROR_FAILED_SERVICE_CONTROLLER_CONNECT;
         }
      }

      return 0;
   }

} //namespace tools
