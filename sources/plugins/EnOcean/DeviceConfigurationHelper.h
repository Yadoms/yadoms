#pragma once
#include <shared/DataContainer.h>
#include "ProfileHelper.h"

//--------------------------------------------------------------
/// \brief	Device configuration helper
//--------------------------------------------------------------
class CDeviceConfigurationHelper
{
public:
   CDeviceConfigurationHelper(const CProfileHelper& profile,
                              const std::string& manufacturerName);
   virtual ~CDeviceConfigurationHelper();

   const shared::CDataContainerSharedPtr& configuration() const;

private:
   shared::CDataContainerSharedPtr m_deviceConfiguration;
};
