#pragma once
#include "ISms.h"

//--------------------------------------------------------------
/// \brief	This class represent a SMS
//--------------------------------------------------------------
class CSms : public ISms
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CSms(const std::string& number, const std::string& content);

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



