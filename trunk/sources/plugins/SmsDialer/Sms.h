#pragma once
#include "ISms.h"
#include <gammu.h>

//--------------------------------------------------------------
/// \brief	This class represent a SMS
//--------------------------------------------------------------
class CSms : public ISms
{  
public:
   //--------------------------------------------------------------
   /// \brief	            Constructor from raw SMS data
   /// \param[in] number   Target recipient phone number
   /// \param[in] content  SMS text
   //--------------------------------------------------------------
   CSms(const std::string& number, const std::string& content);

   //--------------------------------------------------------------
   /// \brief	            Constructor from Gammu data
   /// \param[in] smsGammu Gammu SMS structure
   //--------------------------------------------------------------
   CSms(const GSM_MultiSMSMessage& gammuSms);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSms() {}

   // ISms implementation
   virtual const std::string& getNumber() const;
   virtual const std::string& getContent() const;
   // [END] ISms implementation

private:
   //--------------------------------------------------------------
   /// \brief	The phone number of the sender in case of received message,
   ///         or the recipient in case of sent message
   //--------------------------------------------------------------
   std::string m_number;

   //--------------------------------------------------------------
   /// \brief	The message text
   //--------------------------------------------------------------
   std::string m_content;
};



