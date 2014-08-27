#pragma once

#include <shared/event/EventHandler.hpp>
#include "ZWaveConfiguration.h"

//--------------------------------------------------------------
/// \brief	Interface for ZWave controller
//--------------------------------------------------------------
class IZWaveController
{
public:
   //--------------------------------------------------------------
   /// \brief	Virtual destructor
   //--------------------------------------------------------------
   virtual ~IZWaveController() {}

   //--------------------------------------------------------------
   /// \brief	Start the controller
   /// \param [in] 	configuration  the configuration
   /// \param [in] 	handler        the event handler
   //--------------------------------------------------------------
   virtual void start(CZWaveConfiguration & configuration, shared::event::CEventHandler & handler) = 0;

   //--------------------------------------------------------------
   /// \brief	Stop the controller
   //--------------------------------------------------------------
   virtual void stop() = 0;
};
