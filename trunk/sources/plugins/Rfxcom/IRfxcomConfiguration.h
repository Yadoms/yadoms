#pragma once


//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IRfxcomConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IRfxcomConfiguration() {}

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the rfxcom
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;
};

