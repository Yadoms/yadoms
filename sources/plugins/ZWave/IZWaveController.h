#pragma once

#include <shared/event/EventHandler.hpp>
#include "ZWaveConfiguration.h"
#include "OpenZWaveNode.h"
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
   /// \brief	   The zwave node list type
   //--------------------------------------------------------------
   typedef std::vector<boost::shared_ptr<COpenZWaveNode> > NodeListType;

   //--------------------------------------------------------------
   /// \brief	Virtual destructor
   //--------------------------------------------------------------
   virtual ~IZWaveController() {}

   //--------------------------------------------------------------
   /// \brief	Configure the controller
   /// \param [in] 	configuration  the configuration
   /// \param [in] 	handler        the event handler
   /// \param [in] 	developerMode  indicate if developerMode is active
   //--------------------------------------------------------------
   virtual void configure(CZWaveConfiguration * configuration, shared::event::CEventHandler * handler, bool developerMode) = 0;   
   
   //--------------------------------------------------------------
   /// \brief	Start the controller
   /// \return The start result code
   //--------------------------------------------------------------
   virtual E_StartResult start(boost::function0<void> checkStoprequested) = 0;

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

   //--------------------------------------------------------------
   /// \brief	Ask to device configuration schema of a device
   /// \param [in] 	device   the targetted device
   /// \return The configuration schema
   //--------------------------------------------------------------
   virtual shared::CDataContainerSharedPtr getNodeConfigurationSchema(const std::string & device) = 0;

   //--------------------------------------------------------------
   /// \brief	Update device configuration
   /// \param [in] 	device         The targetted device
   /// \param [in]   configuration  The configuration values
   //--------------------------------------------------------------
   virtual void setNodeConfiguration(const std::string & device, const shared::CDataContainerSharedPtr&configuration) =0;

   //--------------------------------------------------------------
   /// \brief	Update device configuration (self update, from a device notification) => updates only configuration container
   /// \param [in] 	   device         The targetted device
   /// \param [in] 	   keyword        The targetted keyword
   /// \param [in] 	   value          The new value
   /// \param [in-out]  configuration  The initial and output configuration values
   //--------------------------------------------------------------
   virtual void updateNodeConfiguration(const std::string & device, const std::string& keyword, const std::string& value, shared::CDataContainerSharedPtr& configuration) = 0;

   //--------------------------------------------------------------
   /// \brief	Get all the nodes
   /// \return The node list
   //--------------------------------------------------------------
   virtual NodeListType & getNodeList() = 0;

   //--------------------------------------------------------------
   /// \brief	Call an extra query on a device
   /// \param [in] 	   device         The targeted device
   /// \param [in] 	   keyword        The extra query
   /// \param [in] 	   value          The extra query data
   /// \return true if successfull
   //--------------------------------------------------------------
   virtual bool onDeviceExtraQuery(const std::string & targetDevice, const std::string & extraQuery, const shared::CDataContainerSharedPtr&data) = 0;

};
