#include "stdafx.h"
#include "ApplicationLoader.h"
#include "service/ApplicationService.h"
#include "service/ServiceInstaller.h"
#include "service/ServiceInstallerProgramOptions.h"
#include <shared/Log.h>

namespace tools {

   int CApplicationLoader::run(IApplication & application, service::IApplicationServiceInformation & info, int argc, char ** argv)
   {
      service::CServiceInstallerProgramOptions po(argc, argv, info);

      if (po.getInstallService())
      {
         // Install the service when the command is 
         // "-install" or "/install".
         service::CServiceInstaller::InstallService(
            po.getServiceName(),                // Name of service
            po.getServiceDisplayName(),         // Name to display
            po.getStartType(),                  // Service start type
            po.getServiceDependencies(),        // Dependencies
            po.getServiceAccount(),             // Service running account
            po.getServiceAccountPassword(),     // Password of the account
            po.getRemainingArguments()
            );

      }
      else if (po.getRemoveService())
      {
         // Uninstall the service when the command is 
         // "-remove" or "/remove".
         service::CServiceInstaller::UninstallService(po.getServiceName());
      }
      else
      {
         //find the working dir (= path to exe)
         boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
         full_path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));

         service::CApplicationService service(application, po.getServiceName(), full_path.parent_path());
         if (!service::CServiceBase::Run(service))
         {
            if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
            {
               YADOMS_LOG(info) << "Running Yadoms as console application";
               //running from console, just start application without service behavior
               return application.run(argc, argv);
            }
            else
            {
               YADOMS_LOG(fatal) << boost::format("Service failed to run w / err 0x%08X") % ERROR_FAILED_SERVICE_CONTROLLER_CONNECT;
            }
         }
      }

      return 0;
   }

} //namespace tools
