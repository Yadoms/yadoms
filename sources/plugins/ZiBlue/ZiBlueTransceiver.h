#pragma once

#include <shared/communication/StringBuffer.h>

class CZiBlueTransceiver 
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   CZiBlueTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CZiBlueTransceiver();

   shared::communication::CStringBuffer buildHelloCmd() const;
};

