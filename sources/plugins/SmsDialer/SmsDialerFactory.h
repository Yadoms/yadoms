#pragma once

#include "ISmsDialerConfiguration.h"
#include "IPhone.h"

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CSmsDialerFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmsDialerFactory();

   //--------------------------------------------------------------
   /// \brief	    Create a phone instance and connect to phone
   /// \param[in] configuration  Plugin instance configuration (contains the serial port)
   /// \throw      CPhoneException if error initializing or phone connecting
   //--------------------------------------------------------------
   static boost::shared_ptr<IPhone> constructPhone(const ISmsDialerConfiguration& configuration);
};

