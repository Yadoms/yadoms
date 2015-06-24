#include "stdafx.h"
#include "OperatingSystemIntegration.h"
#include "ConsoleControlHandler.h"
#include "ServiceControlHandler.h"
namespace tools {

   COperatingSystemIntegration::COperatingSystemIntegration()
   {
   }
   
   COperatingSystemIntegration::~COperatingSystemIntegration()
   {
   }
   
   void COperatingSystemIntegration::configure()
   {
      CConsoleControlHandler::ConfigureConsoleControl();
      CServiceControlHandler::ConfigureServiceControl();
   }
      
} //namespace tools
