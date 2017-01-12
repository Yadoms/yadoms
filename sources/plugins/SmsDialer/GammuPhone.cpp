#include "stdafx.h"
#include <shared/Log.h>
#include "GammuPhone.h"
#include "PhoneException.hpp"
#include "Sms.h"

CGammuPhone::CGammuPhone(const ISmsDialerConfiguration& configuration)
   : m_configuration(configuration),
   m_connection(configuration),
   m_smsSendStatus(ERR_NONE),
   m_incompleteMessageId(-1),
   m_incompleteMessageTime(0),
   m_on(false)
{
}

CGammuPhone::~CGammuPhone()
{
}

void CGammuPhone::handleGammuError(GSM_Error gsmError,
                                   const std::string& errorMessage)
{
   if (gsmError != ERR_NONE)
      throw CPhoneException(std::string("Phone communication : ") + errorMessage + std::string(" : ") + std::string(GSM_ErrorString(gsmError)));
}

bool CGammuPhone::connect()
{
   m_phoneId.clear();

   if (!m_connection.connect())
      return false;

   // Read some phone informations
   char readValue[MAX(GSM_MAX_MANUFACTURER_LENGTH, GSM_MAX_MODEL_LENGTH) + 1];

   // Manufacturer
   handleGammuError(GSM_GetManufacturer(m_connection.getGsmContext(), readValue), "Unable to get phone manufacturer");
   m_phoneId.append(readValue);

   // Model name
   handleGammuError(GSM_GetModel(m_connection.getGsmContext(), readValue), "Unable to get phone model");
   GSM_PhoneModel* model = GSM_GetModelInfo(m_connection.getGsmContext());
   if (!model)
      handleGammuError(ERR_UNKNOWNMODELSTRING, "Unable to get phone model info");
   m_phoneId.append(", ").append(model->model);
   m_phoneId.append(" (").append(readValue).append(") ");
   m_phoneId.append(model->number);

   YADOMS_LOG(information) << "Phone found : " << m_phoneId ;

   return true;
}

bool CGammuPhone::isConnected() const
{
   return m_connection.isConnected();
}

void CGammuPhone::powerOn(bool on)
{
   GSM_SetPower(m_connection.getGsmContext(), on ? TRUE : FALSE);
   m_on = on;
}

bool CGammuPhone::isOn() const
{
   return m_on;
}

const std::string& CGammuPhone::getUniqueId() const
{
   return m_phoneId;
}

void CGammuPhone::unlock(const std::string& pin)
{
   GSM_SecurityCode SecurityCode;
   handleGammuError(GSM_GetSecurityStatus(m_connection.getGsmContext(), &SecurityCode.Type), "Unable to get the phone security status");

   // Check phone security mode
   switch (SecurityCode.Type)
   {
   case SEC_None: // Phone is not locked
      return;
   case SEC_Pin: // Phone is locked by PIN code
      break;
   default:
      throw CPhoneException(std::string("Phone security mode not supported : ") + boost::lexical_cast<std::string>(SecurityCode.Type));
   }

   // Check PIN validity
   if (!regex_match(pin, boost::regex("[0-9]{4}")))
      throw CPhoneException(std::string("Can not unlock phone, because PIN code is invalid (must be 4 digits)"));

   // Unlock the phone
   strcpy(SecurityCode.Code, pin.c_str()); // strcpy is safe here because 'pin' length is 4 (just validated by the regex)
   handleGammuError(GSM_EnterSecurityCode(m_connection.getGsmContext(), &SecurityCode), "Unable to unlock the phone");
}

void CGammuPhone::send(boost::shared_ptr<ISms> sms)
{
   if (!isConnected())
      throw CPhoneException("Phone must be connected to send SMS");

   YADOMS_LOG(information) << "Send SMS to number " << sms->getNumber() << " \"" << sms->getContent() << "\"" ;

   // Fill in SMS info structure which will be used to generate messages.
   GSM_MultiPartSMSInfo SMSInfo;
   boost::shared_ptr<unsigned char> messageUnicode(new unsigned char[(sms->getContent().size() + 1) * 2]);
   GSM_ClearMultiPartSMSInfo(&SMSInfo);
   SMSInfo.Class = 1; // Class 1 message (normal)
   SMSInfo.EntriesNum = 1; // Message will be consist of one part
   SMSInfo.UnicodeCoding = FALSE; // No unicode
   SMSInfo.Entries[0].ID = SMS_ConcatenatedTextLong; // The part has type long text
   EncodeUnicode(messageUnicode.get(), sms->getContent().c_str(), sms->getContent().size()); // Encode message text
   SMSInfo.Entries[0].Buffer = messageUnicode.get();

   // Encode message into PDU parts
   GSM_MultiSMSMessage gammuSms;
   handleGammuError(GSM_EncodeMultiPartSMS(nullptr, &SMSInfo, &gammuSms), "Sending SMS : Unable to encode message into PDU parts");

   // Set callback for message sending
   // This needs to be done after connection initialized
   GSM_SetSendSMSStatusCallback(m_connection.getGsmContext(), sendSmsCallback, this);

   // Get the SMSC number from phone
   GSM_SMSC PhoneSMSC;
   PhoneSMSC.Location = 1; // First position in the SIM
   handleGammuError(GSM_GetSMSC(m_connection.getGsmContext(), &PhoneSMSC), "Sending SMS : Unable to get the SMS center number (SMSC) from the SIM. Check the phone configuration, and try to send SMS manually from phone.");

   // Send message parts
   for (int partIndex = 0; partIndex < gammuSms.Number; ++partIndex)
   {
      // Set the SMSC number in message
      CopyUnicodeString(gammuSms.SMS[partIndex].SMSC.Number, PhoneSMSC.Number);

      // Prepare message
      // Encode recipient number
      EncodeUnicode(gammuSms.SMS[partIndex].Number, sms->getNumber().c_str(), sms->getNumber().size());
      // It's a outgoing message
      gammuSms.SMS[partIndex].PDU = SMS_Submit;

      // Send message
      handleGammuError(GSM_SendSMS(m_connection.getGsmContext(), &gammuSms.SMS[partIndex]), "Sending SMS : Fail to send SMS.");

      // Set flag before calling SendSMS, some phones might give
      // instant response
      m_smsSendStatus = ERR_TIMEOUT;

      while (m_smsSendStatus == ERR_TIMEOUT)
      {
         // Wait for network reply
         // Result is returned by the sendSmsCallback function
         GSM_ReadDevice(m_connection.getGsmContext(), true);

#undef sleep // Need because Gammu defined 'sleep' as macro, that prevent to use boost::this_thread method

         // Give a chance to stop (by boost::thread_interrupted exception)
         boost::this_thread::sleep(boost::posix_time::milliseconds(0));
      }

      if (m_smsSendStatus != ERR_NONE)
      {
         // Message sending failed
         throw CPhoneException(std::string("SMS send report : error "));
      }
   }

   // Message sent OK
   YADOMS_LOG(information) << "SMS sent successfully" ;
}

void CGammuPhone::sendSmsCallback(GSM_StateMachine* sm, int status, int MessageReference, void* user_data)
{
   auto instance = static_cast<CGammuPhone*>(user_data);

   if (status != 0)
   {
      YADOMS_LOG(error) << "SMS send report : error " << boost::lexical_cast<int>(status) ;
      instance->m_smsSendStatus = ERR_UNKNOWN;
   }
   else
   {
      instance->m_smsSendStatus = ERR_NONE;
   }
}

boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > CGammuPhone::getIncomingSMS()
{
   if (!isConnected())
      throw CPhoneException("Phone must be connected to read incoming SMS");

   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > noSms;
   bool newSms;

   GSM_SMSMemoryStatus gammuSmsStatus;
   GSM_Error gammuError = GSM_GetSMSStatus(m_connection.getGsmContext(), &gammuSmsStatus);
   switch (gammuError)
   {
   case ERR_NONE:
   {
      newSms = (gammuSmsStatus.SIMUsed + gammuSmsStatus.PhoneUsed > 0);
      break;
   }
   case ERR_NOTSUPPORTED:
   case ERR_NOTIMPLEMENTED:
   {
      // Not supported, try another method
      GSM_MultiSMSMessage gammuSms;
      gammuSms.Number = 0;
      gammuSms.SMS[0].Location = 0;
      gammuSms.SMS[0].Folder = 0;
      newSms = (GSM_GetNextSMS(m_connection.getGsmContext(), &gammuSms, TRUE) == ERR_NONE);
      break;
   }
   default:
   {
      // Error
      YADOMS_LOG(error) << "Error getting SMS status : " << GSM_ErrorString(gammuError) ;
      return noSms;
   }
   }

   // No SMS found in phone
   if (!newSms)
      return noSms;

   // Read found SMS
   return readSms(true);
}

boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > CGammuPhone::readSms(bool deleteSms)
{
   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > noSms;

   GSM_MultiSMSMessage gammuSms;
   std::vector<GSM_MultiSMSMessage> gammuSmsVector;
   GSM_Error gammuError = ERR_NONE;

   gboolean start = TRUE;
   while (gammuError == ERR_NONE)
   {
      gammuSms.SMS[0].Folder = 0;
      gammuError = GSM_GetNextSMS(m_connection.getGsmContext(), &gammuSms, start);

      switch (gammuError)
      {
      case ERR_EMPTY:
      {
         // No more sms
         break;
      }
      case ERR_NONE:
      {
         if (isValidMessage(&gammuSms))
            gammuSmsVector.push_back(gammuSms);
         break;
      }
      default:
      {
         YADOMS_LOG(error) << "Error getting SMS : " << gammuError ;
         return noSms;
      }
      }

      start = FALSE;
   }

   // Now we have to regroup SMS (if some were parted)
   // Convert from vector to array of pointers (as expected by GSM_LinkSMS function)
   boost::shared_ptr<GSM_MultiSMSMessage*> gammuSmsPtrArray(new GSM_MultiSMSMessage*[gammuSmsVector.size() + 1]);
   size_t gammuSmsPtrArrayIndex = 0;
   for (auto it = gammuSmsVector.begin(); it != gammuSmsVector.end(); ++it)
   {
      gammuSmsPtrArray.get()[gammuSmsPtrArrayIndex++] = &(*it);
   }
   gammuSmsPtrArray.get()[gammuSmsPtrArrayIndex] = nullptr;

   boost::shared_ptr<GSM_MultiSMSMessage*> gammuSortedSmsPtrArray(new GSM_MultiSMSMessage*[gammuSmsVector.size() + 1]);
   if (GSM_LinkSMS(nullptr, gammuSmsPtrArray.get(), gammuSortedSmsPtrArray.get(), TRUE) != ERR_NONE)
   {
      YADOMS_LOG(error) << "Error getting SMS : " << gammuError ;
      return noSms;
   }

   // Process messages
   boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > smsList(new std::vector<boost::shared_ptr<ISms> >);
   for (size_t idxSms = 0; gammuSortedSmsPtrArray.get()[idxSms] != nullptr; ++idxSms)
   {
      // Check incomplete message (for long parted SMS)
      if (!checkMultipart(gammuSortedSmsPtrArray.get()[idxSms]))
         return noSms;

      // Record the message
      boost::shared_ptr<ISms> sms(new CSms(*gammuSortedSmsPtrArray.get()[idxSms]));
      smsList->push_back(sms);
   }

   // Delete processed messages if requested
   if (deleteSms)
      deleteSmsFromPhone(gammuSortedSmsPtrArray);

   return smsList;
}

void CGammuPhone::deleteSmsFromPhone(boost::shared_ptr<GSM_MultiSMSMessage*> gammuSmsPtrArray)
{
   // Delete each part of each SMS
   for (size_t idxSms = 0; gammuSmsPtrArray.get()[idxSms] != nullptr; ++idxSms)
   {
      for (auto idxPart = 0; idxPart < gammuSmsPtrArray.get()[idxSms]->Number; ++idxPart)
      {
         gammuSmsPtrArray.get()[idxSms]->SMS[idxPart].Folder = 0;
         auto gammuError = GSM_DeleteSMS(m_connection.getGsmContext(), &gammuSmsPtrArray.get()[idxSms]->SMS[idxPart]);
         switch (gammuError)
         {
         case ERR_NONE:
         case ERR_EMPTY:
            break;
         default:
            YADOMS_LOG(error) << "Error deleting SMS : " << gammuError ;
         }
      }
   }
}

bool CGammuPhone::isValidMessage(GSM_MultiSMSMessage* gammuSms)
{
   // SMS is not in Inbox SMS ==> exit
   if (!gammuSms->SMS[0].InboxFolder)
      return false;

   // Message is OK
   YADOMS_LOG(information) << "Received message" ;
   return true;
}

bool CGammuPhone::checkMultipart(GSM_MultiSMSMessage* gammuSms)
{
   /* Does the message have UDH (is multipart)? */
   if (gammuSms->SMS[0].UDH.Type == UDH_NoUDH || gammuSms->SMS[0].UDH.AllParts == -1)
      return true;

   /* Grab current id */
   auto currentId = (gammuSms->SMS[0].UDH.ID16bit != -1) ? gammuSms->SMS[0].UDH.ID16bit : gammuSms->SMS[0].UDH.ID8bit;

   /* Do we have same id as last incomplete? */
   auto sameId = (m_incompleteMessageId != -1 && m_incompleteMessageId == currentId);

   /* Check if we have all parts */
   if (gammuSms->SMS[0].UDH.AllParts == gammuSms->Number)
   {
      if (sameId)
         razMultipartWaitFlags();
      return true;
   }

   /* Have we seen this message recently? */
   if (sameId)
   {
      static const double multipartTimeout = 600; // 600 seconds to retrieve all parts os a message
      if (m_incompleteMessageTime != 0 && difftime(time(nullptr), m_incompleteMessageTime) >= multipartTimeout)
      {
         // Incomplete multipart message processing after timeout
         razMultipartWaitFlags();
         return true;
      }
      // Incomplete multipart message, waiting for other parts
   }
   else
   {
      if (m_incompleteMessageTime == 0)
      {
         // Incomplete multipart message, waiting for other parts
         m_incompleteMessageId = (gammuSms->SMS[0].UDH.ID16bit != -1) ? gammuSms->SMS[0].UDH.ID16bit : gammuSms->SMS[0].UDH.ID8bit;
         m_incompleteMessageTime = time(nullptr);
      }
      else
      {
         // Incomplete multipart message, but waiting for other one
      }
   }

   // Incomplete multipart message
   return false;
}

void CGammuPhone::razMultipartWaitFlags()
{
   m_incompleteMessageTime = 0;
   m_incompleteMessageId = -1;
}

