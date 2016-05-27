#include "stdafx.h"
#include "OperatingSystemIntegration.h"
#include "ConsoleControlHandler.h"
#include "ServiceControlHandler.h"
//TODO virer
namespace shared
{
   namespace process
   {
      COperatingSystemIntegration::COperatingSystemIntegration()
      {
      }

      COperatingSystemIntegration::~COperatingSystemIntegration()
      {
      }

      void COperatingSystemIntegration::setApplicationStopRequestHandler(boost::function<void(boost::function<void()>)> onStopRequestedFct)
      {
         CConsoleControlHandler::setOnStopRequestedHandler(onStopRequestedFct);
            CServiceControlHandler::setOnStopRequestedHandler(onStopRequestedFct);
      }
   }
} // namespace shared::process
