#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/xpl/XplMessage.h>
#include <shared/event/EventHandler.hpp>
#include "SmsDialerConfiguration.h"
#include "IPhone.h"

//--------------------------------------------------------------
/// \brief	This class is a simple plugin example
/// \note   This plugin do nothing useful, just demonstrate how to :
///         - use a configuration
///         - receive a XPL message
///         - send a XPL message
//--------------------------------------------------------------
class CSmsDialer : public shared::event::CEventHandler, public shared::plugin::IPlugin
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CSmsDialer();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSmsDialer();

   // IPlugin implementation
   virtual void doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService);
   virtual void updateConfiguration(const std::string& configurationValues);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Process the not connected state (wait for connection)
   //--------------------------------------------------------------
   void ProcessNotConnectedState();

   //--------------------------------------------------------------
   /// \brief	                     Process the connected state (Treat XPL messages, send SMS...)
   //--------------------------------------------------------------
   void ProcessConnectedState();

   //--------------------------------------------------------------
   /// \brief	                     Called when an XPL message is received
   /// \param [in] xplMessage       The received message
   //--------------------------------------------------------------
   void onXplMessageReceived(const shared::xpl::CXplMessage& xplMessage);

   //--------------------------------------------------------------
   /// \brief	                     Check if incomming SMS and process it
   //--------------------------------------------------------------
   void processIncommingSMS();

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CSmsDialerConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The phone used to send/receive SMS
   //--------------------------------------------------------------
   boost::shared_ptr<IPhone> m_phone;

   //--------------------------------------------------------------
   /// \brief	Timer to try connection to phone
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_connectionTimer;

   //--------------------------------------------------------------
   /// \brief	Timer to poll for incomming SMS
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_incommingSmsPollTimer;
};



