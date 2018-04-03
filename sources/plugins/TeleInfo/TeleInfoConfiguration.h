#pragma once

#include "ITeleInfoConfiguration.h"

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
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSerialPort() const override;
   EEquipmentType getEquipmentType() const override;
   // [END] ITeleInfoConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};