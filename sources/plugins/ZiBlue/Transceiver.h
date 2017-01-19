#pragma once

#include <shared/communication/StringBuffer.h>

class CTransceiver 
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   CTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Get the hello command
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildHelloCmd() const;

   //--------------------------------------------------------------
   /// \brief	                           Get the start listening command
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildStartListeningData() const;

   
};

