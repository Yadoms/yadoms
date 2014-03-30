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
   /// \throw      CPhoneException if error initializing or phone connecting
   //--------------------------------------------------------------
   static boost::shared_ptr<IPhone> constructPhone(const ISmsDialerConfiguration& configuration);
};

