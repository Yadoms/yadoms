#pragma once
#include <shared/plugin/IPlugin.h>
#include "SmsDialerConfiguration.h"
#include "IPhone.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

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
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Process the not connected state (wait for connection)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processNotConnectedState(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Process the connected state (Check for incomming SMS, send SMS...)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processConnectedState(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief                       Declare the device for Yadoms
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief                       Process the phone power on/off request
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] powerRequest     Details of the request
   //--------------------------------------------------------------
   void onPowerPhoneRequest(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& powerRequest);

   //--------------------------------------------------------------
   /// \brief	                     Process a sending message request
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] sendSmsRequest   Details of the request
   //--------------------------------------------------------------
   void onSendSmsRequest(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& sendSmsRequest);

   //--------------------------------------------------------------
   /// \brief	                     Check if incoming SMS and process it
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processIncommingSMS(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Send an acknowledge to Yadoms
   /// \param [in] ok               true for positive ack, false for negative ack
   //--------------------------------------------------------------
   void notifyAck(bool ok) const;

   //--------------------------------------------------------------
   /// \brief	                     Send the phone power state to Yadoms
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] on               true if power is on
   //--------------------------------------------------------------
   void notifyPhonePowerState(boost::shared_ptr<yApi::IYadomsApi> context, bool on) const;

   //--------------------------------------------------------------
   /// \brief	                     Send the received SMS to Yadoms
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] sms              sms to transmit to Yadoms
   //--------------------------------------------------------------
   void notifySmsReception(boost::shared_ptr<yApi::IYadomsApi> context, const boost::shared_ptr<ISms>& sms) const;

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



