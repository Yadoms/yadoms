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

   std::string buildHelloCmd() const;
   std::string buildStartListeningData() const;

   
};

