#pragma once
#include <shared/plugin/IPlugin.h>
#include "RfxLanXplConfiguration.h"
#include "xplcore/XplMessage.h"
#include "xplcore/XplService.h"
#include "xplcore/XplHub.h"
#include "xplrules/IDeviceManager.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

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
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
   // [END] IPlugin implementation

private:
   //----------------------------------------------
   ///\brief Function handler when receiving XplMessage
   ///\param [in] message The xpl message received
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnXplMessageReceived(xplcore::CXplMessage & message, boost::shared_ptr<yApi::IYPluginApi> context);

   //----------------------------------------------
   ///\brief Function handler use to send a command device
   ///\param [in] message The xpl message received
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnSendDeviceCommand(boost::shared_ptr<const yApi::IDeviceCommand> command, boost::shared_ptr<yApi::IYPluginApi> context, xplcore::CXplService & xplService);

   //----------------------------------------------
   ///\brief Function handler use to create a device from a user configuration
   ///\param [in] configuration  the device configuration
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnCreateDeviceRequest(boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> data, boost::shared_ptr<yApi::IYPluginApi> context);

	//----------------------------------------------
   ///\brief Function handler use to answer custom queries
   ///\param [in] data  the custom query data
   ///\param [in] context The plugin API
   //----------------------------------------------
   void OnBindingQueryRequest(boost::shared_ptr<yApi::IBindingQueryRequest> data, boost::shared_ptr<yApi::IYPluginApi> context);


   //----------------------------------------------
   ///\brief Function that start a peripheral listing (send hbeat.request)
   ///\param [in] xplService  the xpl service
   //----------------------------------------------
   void StartPeripheralListing(xplcore::CXplService & xplService);


   //----------------------------------------------
   ///\brief Function that start the hub
   ///\param [in] hubFilterConfiguration  the hub configuration
   //----------------------------------------------
   void startHub(const std::string & hubFilterConfiguration); 
   
   //----------------------------------------------
   ///\brief Function that stop the hub
   //----------------------------------------------
   void stopHub();

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

   //--------------------------------------------------------------
   /// \brief	The list of all rfxlan which have been connected once
   //--------------------------------------------------------------
   std::vector<std::string> m_connectedRfxLans;

   boost::shared_ptr<xplcore::CXplHub> m_hub;
};



