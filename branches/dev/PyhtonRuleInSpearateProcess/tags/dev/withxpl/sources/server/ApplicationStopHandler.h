//
// System signals handler
//
#pragma once

#include <csignal>

//-----------------------------------------------------------------------------
/// \class              Application stop handler
//-----------------------------------------------------------------------------
class CApplicationStopHandler
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		         Configure application stop handler
   //-----------------------------------------------------------------------------
   static void configure();

   //-----------------------------------------------------------------------------
   /// \brief		         Indicate if stop was requested
   /// \return             true is stop was requested
   //-----------------------------------------------------------------------------
   static bool stopRequested();

private:
   //-----------------------------------------------------------------------------
   /// \brief		   Internal interruption handler
   /// \par signal   signal
   //-----------------------------------------------------------------------------
   static void handleInternal(int signal);


   static volatile sig_atomic_t StopRequested;
};
