#pragma once
#include <shared/plugin/IPlugin.h>
#include "RfxcomConfiguration.h"
#include "Transceiver.h"
#include "IPort.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This plugin support the RFXCom module (see http://rfxcom.com/)
//--------------------------------------------------------------
class CRfxcom : public shared::plugin::IPlugin
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CRfxcom();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcom();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IDeviceCommand> command);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes connected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes unconnected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received by the RFXCom
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   //--------------------------------------------------------------
   void processRfxcomDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& data);

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CRfxcomConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The port used to communicate with RFXcom
   //--------------------------------------------------------------
   boost::shared_ptr<IPort> m_port;

   //--------------------------------------------------------------
   /// \brief	The RFXCom protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<ITransceiver> m_transceiver;
};



