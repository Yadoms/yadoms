#pragma once
#include <shared/plugin/IPlugin.h>
#include "RfxLanXplConfiguration.h"
#include "xplcore/XplMessage.h"
#include "xplcore/XplService.h"
#include "xplrules/IDeviceManager.h"
#include <shared/plugin/yadomsApi/ManuallyDeviceCreationRequest.h>


// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This class is a simple plugin example
/// \note   This plugin do nothing useful, just demonstrate how to :
///         - use a configuration
///         - process a command from Yadoms
///         - send sensor state to Yadoms
//--------------------------------------------------------------
class CRfxLanXpl : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CRfxLanXpl();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRfxLanXpl();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

private:
   //----------------------------------------------
   ///\brief Function handler when receiving XplMessage
   ///\param [in] message The xpl message received
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnXplMessageReceived(xplcore::CXplMessage & message, boost::shared_ptr<yApi::IYadomsApi> context);

   //----------------------------------------------
   ///\brief Function handler use to send a command device
   ///\param [in] message The xpl message received
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnSendDeviceCommand(boost::shared_ptr<const yApi::IDeviceCommand> command, boost::shared_ptr<yApi::IYadomsApi> context, xplcore::CXplService & xplService);

   //----------------------------------------------
   ///\brief Function handler use to create a device from a user configuration
   ///\param [in] configuration  the device configuration
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnCreateDeviceRequest(yApi::CManuallyDeviceCreationRequest data, boost::shared_ptr<yApi::IYadomsApi> context);

   //----------------------------------------------
   ///\brief  The device manager
   //----------------------------------------------
   boost::shared_ptr<xplrules::IDeviceManager>   m_deviceManager;

   //--------------------------------
   ///\brief Instance manager
   //--------------------------------
   xplrules::CRuleInstanceManager m_instanceManager;

   //----------------------------------------------
   ///\brief  The plugin xpl device id
   //----------------------------------------------
   static std::string m_xpl_gateway_id;


   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CRfxLanXplConfiguration m_configuration;
};



