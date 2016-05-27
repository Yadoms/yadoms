#include "stdafx.h"
#include "ApplicationStopHandler.h"
#include "ServiceControlHandler.h"
#include "ConsoleControlHandler.h"

namespace shared
{
   namespace process
   {
      CApplicationStopHandler::CApplicationStopHandler(bool isRunningAsService)
         :m_isRunningAsService(isRunningAsService)
      {
      }

      CApplicationStopHandler::~CApplicationStopHandler()
      {
      }

      void CApplicationStopHandler::setApplicationStopHandler(boost::function<bool()> onStopRequestedFct)
      {
         CConsoleControlHandler::setOnStopRequestedHandler(onStopRequestedFct);
         if (m_isRunningAsService)
            CServiceControlHandler::setOnStopRequestedHandler(onStopRequestedFct);
      }
   }
} // namespace shared::process
