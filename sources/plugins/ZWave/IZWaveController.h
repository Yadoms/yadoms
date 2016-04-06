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

   //--------------------------------------------------------------
   /// \brief	Send a command to a device
   /// \param [in] 	device   the targetted device
   /// \param [in] 	keyword  the targetted keyword
   /// \param [in] 	value    the value
   //--------------------------------------------------------------
   virtual void sendCommand(const std::string & device, const std::string & keyword, const std::string & value) = 0;

   //--------------------------------------------------------------
   /// \brief	Start the inclusion mode
   //--------------------------------------------------------------
   virtual void startInclusionMode() = 0;

   //--------------------------------------------------------------
   /// \brief	Start the exclusion mode
   //--------------------------------------------------------------
   virtual void startExclusionMode() = 0;

   //--------------------------------------------------------------
   /// \brief	Reset controller and erase network configuration (all associations)
   //--------------------------------------------------------------
   virtual void hardResetController() = 0;

   //--------------------------------------------------------------
   /// \brief	Reset controller without erasing network (reboot controller)
   //--------------------------------------------------------------
   virtual void softResetController() = 0;

   //--------------------------------------------------------------
   /// \brief	Test the network
   //--------------------------------------------------------------
   virtual void testNetwork() = 0;

};
