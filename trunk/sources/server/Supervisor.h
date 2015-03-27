#pragma once

#include <shared/event/EventHandler.hpp>
#include "ApplicationStopHandler.h"
#include "startupOptions/IStartupOptions.h"
#include <Poco/Thread.h>
#include <Poco/Runnable.h>

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
   /// \brief		                        Constructor
   /// \param[in] applicationEventHandler Application event handler
   /// \param[in] applicationStopCode     Code for application notification
   //-----------------------------------------------------------------------------
   CSupervisor(boost::shared_ptr<shared::event::CEventHandler> applicationEventHandler, const int applicationStopCode);


   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CSupervisor();

   //-----------------------------------------------------------------------------
   /// \brief		                     The main method (blocking, returns at Yadoms exit)
   //-----------------------------------------------------------------------------
   virtual void run();

   //-----------------------------------------------------------------------------
   /// \brief  Ask the supervisor to stop
   //-----------------------------------------------------------------------------
   void requestToStop();

private:
   //-----------------------------------------------------------------------------
   /// \brief		                     The supervisor event handler
   //-----------------------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventHandler> m_EventHandler;

   //-----------------------------
   ///\brief Event handler for application
   //-----------------------------
   boost::shared_ptr<shared::event::CEventHandler> m_applicationEventHandler;

   //-----------------------------
   ///\brief Event code used to notify application that supervisor ends
   //-----------------------------
   const int m_applicationStopCode;


};

