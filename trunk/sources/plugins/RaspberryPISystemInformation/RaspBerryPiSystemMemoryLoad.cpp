#include "stdafx.h"
#include "RaspBerryPiSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

#include "sys/types.h"
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

CRaspBerryPiSystemMemoryLoad::CRaspBerryPiSystemMemoryLoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_memoryLoad(0), m_Capacity("MemoryLoad"), m_Keyword("RaspBerryPiMemoryLoad")
{}

CRaspBerryPiSystemMemoryLoad::~CRaspBerryPiSystemMemoryLoad()
{}

const std::string& CRaspBerryPiSystemMemoryLoad::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CRaspBerryPiSystemMemoryLoad::getCapacity() const
{
   return m_Capacity;
}

const std::string& CRaspBerryPiSystemMemoryLoad::getKeyword() const
{
   return m_Keyword;
}

void CRaspBerryPiSystemMemoryLoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, getCapacity()  , getKeyword() , yApi::kGet , yApi::kNumeric, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CRaspBerryPiSystemMemoryLoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, getCapacity()  , m_memoryLoad);
}

double CRaspBerryPiSystemMemoryLoad::getValue()
{
   if (sysinfo (&memInfo)!=0)
   {
      std::stringstream Message; 
      Message << "sysinfo failed !"; 
      throw shared::exception::CException ( Message.str() );
   }

   long long totalVirtualMem = memInfo.totalram;

   totalVirtualMem += memInfo.totalswap;
   totalVirtualMem *= memInfo.mem_unit;

   long long virtualMemUsed = memInfo.totalram - memInfo.freeram;

   YADOMS_LOG(debug) << "virtual memory used  :" << virtualMemUsed;
   YADOMS_LOG(debug) << "total virtual memory :" << totalVirtualMem;

   m_memoryLoad = virtualMemUsed*100/double(totalVirtualMem);

   return m_memoryLoad;
}


