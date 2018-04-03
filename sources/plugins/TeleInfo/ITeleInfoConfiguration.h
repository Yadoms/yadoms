#pragma once
#include <shared/DataContainer.h>

typedef enum {
   OneInput = 0,
   TwoInputs
}EEquipmentType;

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
   {}

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

   //--------------------------------------------------------------
   /// \brief	    Equipment type connected for the teleinfo (1 or 2 inputs)
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual EEquipmentType getEquipmentType() const = 0;
};