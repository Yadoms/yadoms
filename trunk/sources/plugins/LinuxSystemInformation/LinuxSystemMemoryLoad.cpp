#include "stdafx.h"
#include "LinuxSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLinuxSystemMemoryLoad::CLinuxSystemMemoryLoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_memoryLoad(0)
{}

CLinuxSystemMemoryLoad::~CLinuxSystemMemoryLoad()
{}

const std::string& CLinuxSystemMemoryLoad::getDeviceId() const
{
   return m_deviceId;
}

void CLinuxSystemMemoryLoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceId, "LinuxMemoryLoad"  , "MemoryLoad" , yApi::IYadomsApi::kReadOnly , yApi::IYadomsApi::kDecimal, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxMemoryLoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "LinuxMemoryLoad"  , m_memoryLoad);
}

double CLinuxSystemMemoryLoad::getValue()
{


   return 0;
}


