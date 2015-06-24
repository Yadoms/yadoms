#pragma once


//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class ISmsDialerConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~ISmsDialerConfiguration() {}

   //--------------------------------------------------------------
   /// \brief	    Device used for Gammu library (serial port)
   //--------------------------------------------------------------
   virtual std::string getGammuPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Protocol used for Gammu library. Available values are defined in GSM_Connections (gsmstate.c)
   //--------------------------------------------------------------
   virtual std::string getGammuProtocol() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Phone PIN code
   //--------------------------------------------------------------
   virtual std::string getPhonePIN() const = 0;
};

