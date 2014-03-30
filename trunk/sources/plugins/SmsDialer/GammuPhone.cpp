#include "stdafx.h"
#include <shared/Log.h>
#include "GammuPhone.h"
#include "GammuSmsSendReport.hpp"
#include "PhoneException.hpp"


// Event IDs
enum
{
   kEvtSmsSent = shared::event::kUserFirstId,   // Always start from shared::event::CEventHandler::kUserFirstId
};


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
   GSM_SetSendSMSStatusCallback(m_connection.getGsmContext(), sendSmsCallback, this);//TODO : la callback n'a pas l'air utile, à voir...

   // Get the SMSC number from phone
   GSM_SMSC PhoneSMSC;
   PhoneSMSC.Location = 1;       // First position in the SIM
   handleGammuError(GSM_GetSMSC(m_connection.getGsmContext(), &PhoneSMSC), "Sending SMS : Unable to get the SMS center number (SMSC) from the SIM. Check the phone configuration, and try to send SMS manually from phone.");

   // Send message parts
   for (int partIndex = 0; partIndex < sms.Number; ++ partIndex)
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

      // Wait for network reply (GSM_ReadDevice manage a timeout)
      // Result is returned by the sendSmsCallback function
      GSM_ReadDevice(m_connection.getGsmContext(), true);

      switch(waitForEvents(boost::posix_time::milliseconds(1000)))
      {
      case kEvtSmsSent:
         {
            // SMS was sent, check report
            CGammuSmsSendReport smsSendReport = popEvent<CGammuSmsSendReport>();
            if (smsSendReport.getStatus() != 0)
            {
               // Error sending message
               throw CPhoneException(std::string ("SMS send report : error ") + boost::lexical_cast<std::string>(smsSendReport.getStatus()));
            }
            YADOMS_LOG(info) << "SMS send successfully";
            break;
         }
      case shared::event::kTimeout:
         {
            // Timeout sending SMS
            throw CPhoneException("Sending SMS : timeout (no answer from Gammu)");
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id";
            popEvent();    // We need to consume this unknown event
            break;
         }
      }
   }
}

void CGammuPhone::sendSmsCallback(GSM_StateMachine *sm, int status, int MessageReference, void * user_data)
{
   CGammuSmsSendReport smsSendReport(status, MessageReference);
   ((CGammuPhone*)user_data)->sendEvent<CGammuSmsSendReport>(kEvtSmsSent, smsSendReport);
}
