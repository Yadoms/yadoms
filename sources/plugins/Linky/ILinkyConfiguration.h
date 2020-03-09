#pragma once
#include <shared/DataContainer.h>

typedef enum {
   OneInput = 0,
   TwoInputs
}EEquipmentType;

typedef enum {
   Input1Activated = 0,
   Input2Activated,
   AllInputsActivated
}EInputsActivated;

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class ILinkyConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~ILinkyConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainerSharedPtr& data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the Linky reader module
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Equipment type connected for the teleinfo (1 or 2 inputs)
   /// \return     the equipment type
   //--------------------------------------------------------------
   virtual EEquipmentType getEquipmentType() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Input activated for the teleinfo (1 or 2 inputs)
   /// \return     input(s) activated
   //--------------------------------------------------------------
   virtual EInputsActivated getInputsActivated() const = 0;
};

