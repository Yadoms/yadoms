#pragma once

#include <shared/event/EventHandler.hpp>
#include "startupOptions/IStartupOptions.h"
#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include "IApplicationStopHandler.h"

//-----------------------------------------------------------------------------
/// \class              Yadoms supervisor
//-----------------------------------------------------------------------------
class CSupervisor : public Poco::Runnable
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
   CSupervisor(const startupOptions::IStartupOptions& startupOptions, const IApplicationStopHandler & stopHandler);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CSupervisor();

   //-----------------------------------------------------------------------------
   /// \brief		                     The main method (blocking, returns at Yadoms exit)
   //-----------------------------------------------------------------------------
   virtual void run();

   //-----------------------------------------------------------------------------
   /// \brief		                     Stop the supervisor
   //-----------------------------------------------------------------------------
   void requestToStop();

private:
   //-----------------------------------------------------------------------------
   /// \brief		                     The supervisor event handler
   //-----------------------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventHandler> m_EventHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     The stop handler
   //-----------------------------------------------------------------------------
   const IApplicationStopHandler & m_stopHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms startup options
   //-----------------------------------------------------------------------------
   const startupOptions::IStartupOptions& m_startupOptions;

};

