#pragma once

#include <shared/event/EventHandler.hpp>
#include "ApplicationStopHandler.h"
#include "startupOptions/IStartupOptions.h"

//-----------------------------------------------------------------------------
/// \class              Yadoms supervisor
//-----------------------------------------------------------------------------
class CSupervisor
{
private:
   //--------------------------------------------------------------
   /// \brief	Event IDs
   //--------------------------------------------------------------
   enum
   {
      kStopRequested = shared::event::kUserFirstId,   // Yadoms stop was required
      kPluginManagerEvent,                            // Event from plugin manager
      kRuleManagerEvent,                              // Event from automation rules manager
      kSystemEvent,                                   // Event from system
   };

public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Constructor
   /// \param[in] startupOptions       Yadoms startup options
   //-----------------------------------------------------------------------------
   CSupervisor(const startupOptions::IStartupOptions& startupOptions);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CSupervisor();

   //-----------------------------------------------------------------------------
   /// \brief		                     The main method (blocking, returns at Yadoms exit)
   //-----------------------------------------------------------------------------
   void doWork();

   //-----------------------------------------------------------------------------
   /// \brief		                     Stop the supervisor
   //-----------------------------------------------------------------------------
   void requestToStop(boost::function<void()> & callbackAfterStopped);

   //-----------------------------------------------------------------------------
   /// \brief		                     Get the requested stop mode
   //-----------------------------------------------------------------------------
   IApplicationStopHandler::EStopMode stopMode() const;

private:
   //-----------------------------------------------------------------------------
   /// \brief		                     The supervisor event handler
   //-----------------------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventHandler> m_EventHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     The stop handler
   //-----------------------------------------------------------------------------
   CApplicationStopHandler m_stopHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms startup options
   //-----------------------------------------------------------------------------
   const startupOptions::IStartupOptions& m_startupOptions;

   //-----------------------------------------------------------------------------
   /// \brief		                     Stopped callback
   //-----------------------------------------------------------------------------
   boost::function<void()> m_callbackAfterStopped;
};

