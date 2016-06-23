#include "stdafx.h"
#include "SystemFactory.h"

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string & device,
                               const ISIConfiguration& configuration)
{
}

CSystemFactory::~CSystemFactory()
{
}

void CSystemFactory::OnSpeedUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const ISIConfiguration& configuration)
{
}
 
void CSystemFactory::OnSlowUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const ISIConfiguration& configuration)
{
}

void CSystemFactory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const ISIConfiguration& configuration)
{
}
