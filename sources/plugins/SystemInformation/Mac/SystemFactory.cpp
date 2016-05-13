#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api, const std::string & device, const ISIConfiguration& configuration)
    :m_PluginName(device)
{
    
}

CSystemFactory::~CSystemFactory()
{}

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> api , const ISIConfiguration& configuration)
{

}
 
void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> api , const ISIConfiguration& configuration)
{

}

void CSystemFactory::OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> api, const ISIConfiguration& configuration )
{

}
