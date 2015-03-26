//
// System signals handler
//
#pragma once

#include <csignal>
#include <shared/event/EventHandler.hpp>
#include "IApplicationStopHandler.h"

//-----------------------------------------------------------------------------
/// \class              Application stop handler
//-----------------------------------------------------------------------------
class CApplicationStopHandler : public IApplicationStopHandler
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Constructor
   //-----------------------------------------------------------------------------
   CApplicationStopHandler();

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CApplicationStopHandler();

   // IApplicationStopHandler implementation
   virtual void requestToStop(EStopMode stopMode) const;
   // [END] IApplicationStopHandler implementation
};
