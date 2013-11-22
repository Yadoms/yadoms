#include "ApplicationStopHandler.h"

#include <iostream>


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
      std::cout << "CApplicationStopHandler::handleInternal - no handler for #%d signal " << signal << std::endl;
      break;
   }
}