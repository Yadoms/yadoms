//
// System signals handler
//
#pragma once

#include <shared/event/EventHandler.hpp>

//-----------------------------------------------------------------------------
/// \class              Application stop handler
//-----------------------------------------------------------------------------
class IApplicationStopHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	    The stop mode
   //--------------------------------------------------------------
   enum EStopMode
   {
      kYadomsOnly,         ///< Stops only Yadoms
      kStopSystem,         ///< Stops Yadoms + halts system
      kRestartSystem,      ///< Stops Yadoms + reboot system
   };

public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Constructor
   /// \param[in] targetEventHandler   Event handler to notify
   /// \param[in] eventId              Event ID to send when stop occurs
   //-----------------------------------------------------------------------------
   virtual void requestToStop(EStopMode stopMode) = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     Register a callback used when app is about to end
   /// \param[in] callbackAfterStopped Callback
   //-----------------------------------------------------------------------------
   virtual void registerForAppEnds(boost::shared_ptr<shared::event::CEventHandler> & handler, const int code) = 0;
};
