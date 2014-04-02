#include "stdafx.h"
#include <shared/Log.h>
#include "GammuPhone.h"
#include "PhoneException.hpp"


CGammuPhone::CGammuPhone(const ISmsDialerConfiguration& configuration)
   :m_configuration(configuration), m_connection(configuration)
{
   // Read some phone informations
   char readValue[MAX(GSM_MAX_MANUFACTURER_LENGTH, GSM_MAX_MODEL_LENGTH) + 1];

   // Manufacturer
   handleGammuError(GSM_GetManufacturer(m_connection.getGsmContext(), readValue), "Unable to get phone manufacturer");
   YADOMS_LOG(info) << "Manufacturer : " << readValue;

   // Model name
   handleGammuError(GSM_GetModel(m_connection.getGsmContext(), readValue), "Unable to get phone model");
   YADOMS_LOG(info) << "Model         : " << GSM_GetModelInfo(m_connection.getGsmContext())->model << "(" << readValue << ")";
}

CGammuPhone::~CGammuPhone()
{
}

void CGammuPhone::handleGammuError(GSM_Error gsmError, const std::string& errorMessage) const
{
   if (gsmError != ERR_NONE)
      throw CPhoneException(std::string ("Phone communication : ") + errorMessage + std::string(" : ") + std::string(GSM_ErrorString(gsmError)));
}

void CGammuPhone::power(bool on)
{
   ////TODO
   //YADOMS_LOG(debug) << "Power " << (on ? "on" : "off") << " the phone";

   //// Initialization
   //try
   //{
   //   initPhone();
   //}
   //catch (CPhoneException& e)
   //{
   //   YADOMS_LOG(error) << "Unexpected answer from AT Phone : " << e.what();
   //   //TODO que faire si l'init s'est mal passée ?
   //}
}

void CGammuPhone::send(const std::string& targetPhoneNumer, const std::string& text)
{
   YADOMS_LOG(info) << "Send SMS to number " << targetPhoneNumer << " \"" << text << "\"";
   
   // Fill in SMS info structure which will be used to generate messages.
   GSM_MultiPartSMSInfo SMSInfo;
   boost::shared_ptr<unsigned char> messageUnicode(new unsigned char[(text.size() + 1) * 2]);
   GSM_ClearMultiPartSMSInfo(&SMSInfo);
   SMSInfo.Class = 1;                                                // Class 1 message (normal)
   SMSInfo.EntriesNum = 1;                                           // Message will be consist of one part
   SMSInfo.UnicodeCoding = FALSE;                                    // No unicode
   SMSInfo.Entries[0].ID = SMS_ConcatenatedTextLong;                 // The part has type long text
   EncodeUnicode(messageUnicode.get(), text.c_str(), text.size());   // Encode message text
   SMSInfo.Entries[0].Buffer = messageUnicode.get();

   // Encode message into PDU parts
   GSM_MultiSMSMessage sms;
   handleGammuError(GSM_EncodeMultiPartSMS(NULL, &SMSInfo, &sms), "Sending SMS : Unable to encode message into PDU parts");

   // Set callback for message sending
   // This needs to be done after connection initialized
   GSM_SetSendSMSStatusCallback(m_connection.getGsmContext(), sendSmsCallback, this);

   // Get the SMSC number from phone
   GSM_SMSC PhoneSMSC;
   PhoneSMSC.Location = 1;       // First position in the SIM
   handleGammuError(GSM_GetSMSC(m_connection.getGsmContext(), &PhoneSMSC), "Sending SMS : Unable to get the SMS center number (SMSC) from the SIM. Check the phone configuration, and try to send SMS manually from phone.");

   // Send message parts
   for (int partIndex = 0; partIndex < sms.Number ; ++ partIndex)
   {
      // Set the SMSC number in message
      CopyUnicodeString(sms.SMS[partIndex].SMSC.Number, PhoneSMSC.Number);

      // Prepare message
      // Encode recipient number
      EncodeUnicode(sms.SMS[partIndex].Number, targetPhoneNumer.c_str(), targetPhoneNumer.size());
      // It's a outgoing message
      sms.SMS[partIndex].PDU = SMS_Submit;

      // Send message
      handleGammuError(GSM_SendSMS(m_connection.getGsmContext(), &sms.SMS[partIndex]), "Sending SMS : Fail to send SMS.");

      // Set flag before calling SendSMS, some phones might give
		// instant response
      m_smsSendStatus = ERR_TIMEOUT;
      
      while (m_smsSendStatus == ERR_TIMEOUT)
      {
         // Wait for network reply
         // Result is returned by the sendSmsCallback function
         GSM_ReadDevice(m_connection.getGsmContext(), true);

#undef sleep   // Need because Gammu defined 'sleep' as macro, that prevent to use boost::this_thread method
         // Give a chance to stop (by boost::thread_interrupted exception)
         boost::this_thread::sleep(boost::posix_time::milliseconds(0));
      }

      if (m_smsSendStatus != ERR_NONE)
      {
         // Message sending failed
         throw CPhoneException(std::string ("SMS send report : error "));
      }
   }

   // Message sent OK
   YADOMS_LOG(info) << "SMS sent successfully";
   return;
}

void CGammuPhone::sendSmsCallback(GSM_StateMachine *sm, int status, int MessageReference, void * user_data)
{
   CGammuPhone* instance = static_cast< CGammuPhone* > (user_data);

   if (status != 0)
   {
      YADOMS_LOG(error) << "SMS send report : error " << boost::lexical_cast<int>(status);
      instance->m_smsSendStatus = ERR_UNKNOWN;
   }
   else
   {
      instance->m_smsSendStatus = ERR_NONE;
   }
}
