#include "stdafx.h"
#include "SystemFactory.h"

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string & device,
                               const ISIConfiguration& configuration,
                               boost::shared_ptr<shared::CDataContainer> details)
{
}

CSystemFactory::~CSystemFactory()
{
}


void CSystemFactory::OnHighFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   
}


void CSystemFactory::OnLowFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                          const ISIConfiguration& configuration)
{
   
}

void CSystemFactory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                           const ISIConfiguration& configuration,
                           boost::shared_ptr<shared::CDataContainer> details)
{
   
}