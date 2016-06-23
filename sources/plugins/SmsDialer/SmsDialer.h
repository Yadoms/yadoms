#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "SmsDialerConfiguration.h"
#include "IPhone.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This plugin can send and receive SMS using a connected phone
//--------------------------------------------------------------
class CSmsDialer : public plugin_cpp_api::IPlugin
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
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Process the not connected state (wait for connection)
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \return                      true if Plugin stop is requested
   //--------------------------------------------------------------
   bool processNotConnectedState(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Process the connected state (Check for incomming SMS, send SMS...)
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \return                      true if Plugin stop is requested
   //--------------------------------------------------------------
   bool processConnectedState(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief                       Declare the device for Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief                       Process the phone power on/off request
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] powerRequest     Details of the request
   //--------------------------------------------------------------
   void onPowerPhoneRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& powerRequest) const;

   //--------------------------------------------------------------
   /// \brief	                     Process a sending message request
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] sendSmsRequest   Details of the request
   //--------------------------------------------------------------
   void onSendSmsRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                         const std::string& sendSmsRequest) const;

   //--------------------------------------------------------------
   /// \brief	                     Check if incoming SMS and process it
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processIncommingSMS(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	                     Send an acknowledge to Yadoms
   /// \param [in] ok               true for positive ack, false for negative ack
   //--------------------------------------------------------------
   static void notifyAck(bool ok);

   //--------------------------------------------------------------
   /// \brief	                     Get the phone number associated to recipient Id
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] recipientId      The recipient ID
   /// \return                      The phone number
   /// \throw shared::exception::CInvalidParameter if recipient ID not found
   //--------------------------------------------------------------
   static std::string getRecipientPhone(boost::shared_ptr<yApi::IYPluginApi> api,
                                        int recipientId);

   //--------------------------------------------------------------
   /// \brief	                     Find the recipient with this phone number
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] phoneNumber      The phone number
   /// \return                      The recipient ID of the recipient found (or the first one if several recipients found)
   /// \throw shared::exception::CInvalidParameter if no recipient found for this phone number
   //--------------------------------------------------------------
   static int findRecipientByPhone(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const std::string& phoneNumber);

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
   /// \brief	    Recipient field used to retrieve phone number from a recipient
   //--------------------------------------------------------------
   static const std::string m_phoneFieldName;

   //--------------------------------------------------------------
   /// \brief	    Message historization object
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CMessage> m_messageKeyword;

   //--------------------------------------------------------------
   /// \brief	    Power historization object
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_powerKeyword;

   //--------------------------------------------------------------
   /// \brief	The keywords list to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
};

