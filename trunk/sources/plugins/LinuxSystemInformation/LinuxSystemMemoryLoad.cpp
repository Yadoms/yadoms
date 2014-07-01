#include "stdafx.h"
#include "LinuxSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

#include "sys/types.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

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
   //context->declareKeyword(m_deviceId, "LinuxMemoryLoad"  , "MemoryLoad" , yApi::IYadomsApi::kReadOnly , yApi::IYadomsApi::kDecimal, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxSystemMemoryLoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "LinuxMemoryLoad"  , m_memoryLoad);
}

double CLinuxSystemMemoryLoad::getValue()
{
//------------------------------------------------------------------
// TODO : A tester
//long long totalPhysMem = memInfo.totalram;
    //Multiply in next statement to avoid int overflow on right hand side...
//    totalPhysMem *= memInfo.mem_unit;

//long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
//physMemUsed *= memInfo.mem_unit;
//------------------------------------------------------------------

   sysinfo (&memInfo);

   long long totalVirtualMem = memInfo.totalram;

   totalVirtualMem += memInfo.totalswap;
   totalVirtualMem *= memInfo.mem_unit;

   long long virtualMemUsed = memInfo.totalram - memInfo.freeram;

   virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
   virtualMemUsed *= memInfo.mem_unit;

   long long totalPhysMem = memInfo.totalram;
   totalPhysMem *= memInfo.mem_unit;

   long long physMemUsed = memInfo.totalram - memInfo.freeram;
   physMemUsed *= memInfo.mem_unit;

   return double((physMemUsed*100)/totalPhysMem);
}


