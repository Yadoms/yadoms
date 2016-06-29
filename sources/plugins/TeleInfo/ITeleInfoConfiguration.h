#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class ITeleInfoConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~ITeleInfoConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the TeleInfo reader module
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;
};

