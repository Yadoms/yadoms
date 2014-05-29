#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplService.h>
#include <shared/event/EventHandler.hpp>
#include "RfxcomConfiguration.h"
#include "Transceiver.h"
#include "IPort.h"

//--------------------------------------------------------------
/// \brief	This plugin support the RFXCom module (see http://rfxcom.com/)
//--------------------------------------------------------------
class CRfxcom : public shared::event::CEventHandler, public shared::plugin::IPlugin, public boost::enable_shared_from_this<CRfxcom>
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
   virtual void doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService);
   virtual void updateConfiguration(const std::string& configurationValues);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Called when an XPL message is received
   /// \param [in] xplMessage       The received message
   //--------------------------------------------------------------
   void onXplMessageReceived(const shared::xpl::CXplMessage& xplMessage);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes connected
   //--------------------------------------------------------------
   void processRfxcomConnectionEvent();

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes unconnected
   //--------------------------------------------------------------
   void processRfxcomUnConnectionEvent();

   //--------------------------------------------------------------
   /// \brief	                     Log event in Yadoms
   //--------------------------------------------------------------
   void LogEvent(const std::string& reason);

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CRfxcomConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The XPL service used to send XPL messages to Yadoms
   //--------------------------------------------------------------
   boost::shared_ptr<shared::xpl::CXplService> m_xplService;

   //--------------------------------------------------------------
   /// \brief	The port used to communicate with RFXcom
   //--------------------------------------------------------------
   boost::shared_ptr<IPort> m_port;

   //--------------------------------------------------------------
   /// \brief	The RFXCom protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<ITransceiver> m_transceiver;
};



