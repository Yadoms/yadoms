#pragma once

#include <shared/communication/Buffer.hpp>

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

   shared::communication::CByteBuffer buildHelloCmd() const;
};

