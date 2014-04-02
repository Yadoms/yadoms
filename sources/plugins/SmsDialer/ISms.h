#pragma once

//--------------------------------------------------------------
/// \brief	This interface represent a SMS
//--------------------------------------------------------------
class ISms
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ISms() {}

   //--------------------------------------------------------------
   /// \brief	   Get the phone number
   /// \return    The phone number of the sender in case of received message,
   ///            or the recipient in case of sent message
   //--------------------------------------------------------------
   virtual const std::string& getNumber() const = 0;

   //--------------------------------------------------------------
   /// \brief	   Get the message text
   /// \return    Message text
   //--------------------------------------------------------------
   virtual const std::string& getContent() const = 0;
};



