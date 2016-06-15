#pragma once

#include <shared/event/EventHandler.hpp>
#include "ZWaveConfiguration.h"

//--------------------------------------------------------------
/// \brief	Interface for ZWave controller
//--------------------------------------------------------------
class IZWaveController
{
public:
   enum E_StartResult
   {
      kSuccess = 0,      /// started successfully
      kSerialPortError,  /// in case of serial port access problem
      kControllerError,  /// in case of controller problem
      kUnknownError      /// in case of any other problem      
   };
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
   /// \return The start result code
   //--------------------------------------------------------------
   virtual E_StartResult start() = 0;

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
   /// \param [in] count   The number of test frame to send
   //--------------------------------------------------------------
   virtual void testNetwork(int count) = 0;

   //--------------------------------------------------------------
   /// \brief	Cancel the current active command
   //--------------------------------------------------------------
   virtual void cancelCurrentCommand() = 0;

   //--------------------------------------------------------------
   /// \brief	Heal network
   //--------------------------------------------------------------
   virtual void healNetwork() = 0;

};
