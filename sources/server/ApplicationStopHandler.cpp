#include "stdafx.h"
#include "ApplicationStopHandler.h"
#include <Poco/Util/ServerApplication.h>
#include <tools/OperatingSystem.h>

CApplicationStopHandler::CApplicationStopHandler()
{
}

CApplicationStopHandler::~CApplicationStopHandler()
{
}

void CApplicationStopHandler::requestToStop(EStopMode stopMode) const
{
   switch (stopMode)
   {
   case IApplicationStopHandler::kStopSystem:
      tools::COperatingSystem::shutdown(false);
      break;
   case IApplicationStopHandler::kRestartSystem:
      tools::COperatingSystem::shutdown(true);
      break;
   }
   Poco::Util::ServerApplication::terminate();
}
