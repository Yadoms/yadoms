#pragma once

//--------------------------------------------------------------
/// \brief	This class has nothing to do with the famous smartphone...
///         This class represent a phone, used to send/receive SMS
//--------------------------------------------------------------
class IPhone
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPhone() {}

   //--------------------------------------------------------------
   /// \brief	                  Power on/off the phone (if supported)
   /// \param [in] on            Power on the phone if true, off else
   //--------------------------------------------------------------
   virtual void power(bool on) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Send a SMS
   /// \param [in] phoneNumer    The receiver phone number
   /// \param [in] text          The SMS message content
   //--------------------------------------------------------------
   virtual void send(const std::string& targetPhoneNumer, const std::string& text) = 0;
};



