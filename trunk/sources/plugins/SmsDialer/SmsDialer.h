#pragma once
#include <shared/plugin/IPlugin.h>
#include "SmsDialerConfiguration.h"
#include "IPhone.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This plugin can send and receive SMS using a connected phone
//--------------------------------------------------------------
class CSmsDialer : public shared::plugin::IPlugin
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
   void onPowerPhoneRequest(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& powerRequest);

   //--------------------------------------------------------------
   /// \brief	                     Process a sending message request
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] sendSmsRequest   Details of the request
   //--------------------------------------------------------------
   void onSendSmsRequest(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& sendSmsRequest);

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

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_device;

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
   /// \brief	    Message historization object
   //--------------------------------------------------------------
   yApi::commands::CMessage m_messageKeyword;

   //--------------------------------------------------------------
   /// \brief	    Power historization object
   //--------------------------------------------------------------
   yApi::commands::CSwitch m_powerKeyword;
};



