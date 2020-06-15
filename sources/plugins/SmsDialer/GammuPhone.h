#pragma once
#include "ISmsDialerConfiguration.h"
#include "GammuPhoneConnection.h"
#include "IPhone.h"
#include <gammu.h>

//--------------------------------------------------------------
/// \brief	Use GAMMU library (http://wammu.eu/) to dial with phone
//--------------------------------------------------------------
class CGammuPhone : public IPhone
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] configuration   Configuration of the plugin
   //--------------------------------------------------------------
   explicit CGammuPhone(const ISmsDialerConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CGammuPhone();

   // IPhone implementation
   bool connect() override;
   bool isConnected() const override;
   void powerOn(bool on) override;
   bool isOn() const override;
   const std::string& getUniqueId() const override;
   void unlock(const std::string& pin) override;
   void send(boost::shared_ptr<ISms> sms) override;
   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > getIncomingSMS() override;
   // [END] IPhone implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                  Gammu error handler
   /// \param[in] gsmError       Gammu error value
   /// \param[in] errorMessage   Error message to pass to exception if gsmError != ERR_NONE
   /// \throw  CPhoneException if error
   //--------------------------------------------------------------
   static void handleGammuError(GSM_Error gsmError, const std::string& errorMessage);

   //--------------------------------------------------------------
   /// \brief	                  Callback called by Gammu when SMS was sent
   /// \param[in] gsmContext     The Gammu context
   /// \param[in] status         SMS sending status
   /// \param[in] MessageReference  Reference on SMS sent
   /// \param[in] user_data      Used here to store this
   //--------------------------------------------------------------
   static void sendSmsCallback(GSM_StateMachine *gsmContext, int status, int MessageReference, void * user_data);

   //--------------------------------------------------------------
   /// \brief	                  Read and delete SMS from phone
   /// \param[in] deleteSms         true to delete the read message
   /// \return                   The list of received SMS
   //--------------------------------------------------------------
   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > readSms(bool deleteSms = true);

   //--------------------------------------------------------------
   /// \brief	                     Delete listed SMS from phone
   /// \param[in] gammuSmsPtrArray  List of SMS (Gammu format) to delete
   //--------------------------------------------------------------
   void deleteSmsFromPhone(boost::shared_ptr<GSM_MultiSMSMessage*> gammuSmsPtrArray);

   //--------------------------------------------------------------
   /// \brief	                  Check if message is valid
   /// \param[in] gammuSms       The message
   /// \return                   true if valid
   //--------------------------------------------------------------
   static bool isValidMessage(GSM_MultiSMSMessage* gammuSms);

   //--------------------------------------------------------------
   /// \brief	                  Check if message is multi-parted
   /// \param[in] gammuSms       The message
   /// \return                   true if multi-parted
   //--------------------------------------------------------------
   bool checkMultipart(GSM_MultiSMSMessage* gammuSms);

   //--------------------------------------------------------------
   /// \brief	                  Raz the multi-parted wait flag
   //--------------------------------------------------------------
   void razMultipartWaitFlags();

private:
   //--------------------------------------------------------------
   /// \brief	   The plugin instance configuration
   //--------------------------------------------------------------
   const ISmsDialerConfiguration& m_configuration;

   //--------------------------------------------------------------
   /// \brief	   Phone connection
   //--------------------------------------------------------------
   CGammuPhoneConnection m_connection;

   //--------------------------------------------------------------
   /// \brief	   SMS Send Gammu status
   //--------------------------------------------------------------
   GSM_Error m_smsSendStatus;

   //--------------------------------------------------------------
   /// \brief	   Phone ID, get when connection is established
   //--------------------------------------------------------------
   std::string m_phoneId;

   //--------------------------------------------------------------
   /// \brief	   Gammu context data : incomplete message ID (Multipart messages processing)
   //--------------------------------------------------------------
   int m_incompleteMessageId;

   //--------------------------------------------------------------
   /// \brief	   Gammu context data : incomplete message time (Multipart messages processing)
   //--------------------------------------------------------------
   time_t m_incompleteMessageTime;

   //--------------------------------------------------------------
   /// \brief	   Phone power status
   //--------------------------------------------------------------
   bool m_on;
};



