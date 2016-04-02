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
   /// \brief	Configure the controller
   /// \param [in] 	configuration  the configuration
   /// \param [in] 	handler        the event handler
   //--------------------------------------------------------------
   virtual void configure(CZWaveConfiguration * configuration, shared::event::CEventHandler * handler) = 0;   
   
   //--------------------------------------------------------------
   /// \brief	Start the controller
   /// \return true if success, false if fails
   //--------------------------------------------------------------
   virtual bool start() = 0;

   //--------------------------------------------------------------
   /// \brief	Stop the controller
   //--------------------------------------------------------------
   virtual void stop() = 0;


   virtual void SendCommand(const std::string & device, const std::string & keyword, const std::string & value) = 0;

   virtual void StartInclusionMode() = 0;
};
