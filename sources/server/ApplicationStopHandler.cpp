#include "stdafx.h"
#include "ApplicationStopHandler.h"
#include <shared/Log.h>

volatile sig_atomic_t CApplicationStopHandler::StopRequested = false;


void CApplicationStopHandler::configure()
{
   signal(SIGINT, handleInternal);   // CTRL+C signal
   signal(SIGTERM,handleInternal);   // Termination request
}

bool CApplicationStopHandler::stopRequested()
{
   return StopRequested ? true : false;
}

void CApplicationStopHandler::handleInternal(int signal)
{
   switch(signal)
   {
   case  SIGINT :
   case  SIGTERM :
      StopRequested = true;
      break;
   default:
      YADOMS_LOG(warning) << "CApplicationStopHandler::handleInternal - no handler for #%d signal " << signal;
      break;
   }
}