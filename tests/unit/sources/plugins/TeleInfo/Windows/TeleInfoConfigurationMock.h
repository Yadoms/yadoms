#include "stdafx.h"
#include "../../../../sources/plugins/TeleInfo/ITeleInfoConfiguration.h"

class TeleInfoConfigurationMock : public ITeleInfoConfiguration
{
public:
   TeleInfoConfigurationMock();

   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   std::string getSerialPort() const override;
   EEquipmentType getEquipmentType() const override;
   EInputsActivated getInputsActivated() const override;
};