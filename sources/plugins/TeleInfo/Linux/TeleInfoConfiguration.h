#pragma once

#include "../ITeleInfoConfiguration.h"

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CTeleInfoConfiguration : public ITeleInfoConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CTeleInfoConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CTeleInfoConfiguration();

   // ITeleInfoConfiguration implementation
   void initializeWith(const shared::CDataContainerSharedPtr& data) override;
   std::string getSerialPort() const override;
   EEquipmentType getEquipmentType() const override;
   EInputsActivated getInputsActivated() const override;
   // [END] ITeleInfoConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr m_data;
};