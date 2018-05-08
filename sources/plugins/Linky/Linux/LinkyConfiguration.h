#pragma once

#include "../ILinkyConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CLinkyConfiguration : public ILinkyConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CLinkyConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinkyConfiguration();

   // ILinkyConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getSerialPort() const override;
   EEquipmentType getEquipmentType() const override;
   EInputsActivated getInputsActivated() const override;   
   // [END] ILinkyConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

