#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplService.h>
#include <shared/event/EventHandler.hpp>
#include "SmsDialerConfiguration.h"
#include "IPhone.h"

//--------------------------------------------------------------
/// \brief	This plugin can send and receive SMS using a connected phone
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
   virtual void doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service& pluginIOService);
   virtual void updateConfiguration(const std::string& configurationValues);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Process the not connected state (wait for connection)
   //--------------------------------------------------------------
   void processNotConnectedState();

   //--------------------------------------------------------------
   /// \brief	                     Process the connected state (Treat XPL messages, send SMS...)
   //--------------------------------------------------------------
   void processConnectedState();

   //--------------------------------------------------------------
   /// \brief	                     Called when a power on/off request is received from XPL
   /// \param [in] xplMessage       The xpl request
   //--------------------------------------------------------------
   void onPowerPhoneXplRequest(const shared::xpl::CXplMessage& xplMessage);

   //--------------------------------------------------------------
   /// \brief	                     Called when a Send SMS request is received from XPL
   /// \param [in] xplMessage       The xpl request
   //--------------------------------------------------------------
   void onSendSmsXplRequest(const shared::xpl::CXplMessage& xplMessage);

   //--------------------------------------------------------------
   /// \brief	                     Check if incoming SMS and process it
   //--------------------------------------------------------------
   void processIncommingSMS();

   //--------------------------------------------------------------
   /// \brief                       Configure the XPL filters (messages to be received by the plugin)
   //--------------------------------------------------------------
   void configureXplFilters();

   //--------------------------------------------------------------
   /// \brief                       Announce the XPL devices associated with the phone
   //--------------------------------------------------------------
   void xplDeclareDevices() const;

   //--------------------------------------------------------------
   /// \brief	                     Announce the main device (used to send/receive SMS)
   //--------------------------------------------------------------
   void xplDeclareMainDevice() const;

   //--------------------------------------------------------------
   /// \brief	                     Announce the on/off device (a switch type device used to power on/off the phone)
   //--------------------------------------------------------------
   void xplDeclareOnOffDevice() const;

   //--------------------------------------------------------------
   /// \brief	                     Send the phone power state to XPL network
   /// \param [in] on               true if poser is on
   //--------------------------------------------------------------
   void xplSendPhonePowerState(bool on) const;

   //--------------------------------------------------------------
   /// \brief	                     Send the acknowledge to XPL network
   /// \param [in] ok               true for positive ack, false for negative ack
   /// \param [in] sourceMsg        Original sent message
   //--------------------------------------------------------------
   void xplSendAck(bool ok, const std::string& sourceMsg) const;

   //--------------------------------------------------------------
   /// \brief	                     Send the received SMS to XPL network
   /// \param [in] sms              sms to transmit to XPL network
   //--------------------------------------------------------------
   void xplSendSmsReceived(const boost::shared_ptr<ISms> sms) const;

   //--------------------------------------------------------------
   /// \brief	                     Send the XPL-trig message to XPL network (low-level method)
   /// \param [in] from             "from" recipient (should be empty for an acknowledge)
   /// \param [in] type             Message type
   /// \param [in] content          Message content
   /// \note See XPL message "message.sms" specification (Yadoms specific message)
   //--------------------------------------------------------------
   void xplSendSmsTrigger(const std::string& from, const std::string& type, const std::string& content) const;

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

   //--------------------------------------------------------------
   /// \brief	The XPL service used to send XPL messages to Yadoms
   //--------------------------------------------------------------
   boost::shared_ptr<shared::xpl::CXplService> m_xplService;
};



