#pragma once
#include "ISms.h"

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
   /// \brief	Establish the phone connection
   /// \return true if phone is connected, false else
   //--------------------------------------------------------------
   virtual bool connect() = 0;

   //--------------------------------------------------------------
   /// \brief	Get the phone connection status
   /// \return true if phone is connected, false else
   //--------------------------------------------------------------
   virtual bool isConnected() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the phone power status
   /// \param[in] on true to power ON the phone, false to power OFF
   /// \return true if phone is powered ON, false if OFF
   //--------------------------------------------------------------
   virtual void powerOn(bool on) = 0;

   //--------------------------------------------------------------
   /// \brief	Get the phone power status
   /// \return true if phone is powered ON, false if OFF
   //--------------------------------------------------------------
   virtual bool isOn() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get the phone unique ID
   /// \return A phone ID composed for example of Manufacturer + model + IMEI
   //--------------------------------------------------------------
   virtual std::string getUniqueId() const = 0;

   //--------------------------------------------------------------
   /// \brief	                  Unlock phone injecting PIN code
   /// \param [in] pin           PIN code
   //--------------------------------------------------------------
   virtual void unlock(const std::string& pin) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Send a SMS
   /// \param [in] sms           The SMS to send
   //--------------------------------------------------------------
   virtual void send(boost::shared_ptr<ISms> sms) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Get the next incoming message
   /// \return                   The list of incoming messages, NULL if none
   //--------------------------------------------------------------
   virtual boost::shared_ptr<std::vector<boost::shared_ptr<ISms> > > getIncomingSMS() = 0;
};



