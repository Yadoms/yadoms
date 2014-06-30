#include "stdafx.h"
#include "LinuxSystemYadomsCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLinuxSystemYadomsCPULoad::CLinuxSystemYadomsCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0)
{
}

CLinuxSystemYadomsCPULoad::~CLinuxSystemYadomsCPULoad()
{}

const std::string& CLinuxSystemYadomsCPULoad::getDeviceId() const
{
   return m_deviceId;
}

void CLinuxSystemYadomsCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   //context->declareKeyword(m_deviceId, "YadomsCPULoad"  , "cpuload" , yApi::IYadomsApi::kReadOnly , yApi::IYadomsApi::kDecimal, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxSystemYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "YadomsCPULoad"  , m_CPULoad);
}

double CLinuxSystemYadomsCPULoad::getValue()
{
        return 0;
}
