#include "stdafx.h"
#include "WindowsSystemCPULoad.h"
#include <shared/exception/Exception.hpp>

#pragma comment(lib, "pdh.lib")

CWindowsSystemCPULoad::CWindowsSystemCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0)
{
   PDH_STATUS Status;

   // Create the Query

   Status = PdhOpenQuery(NULL, NULL, &cpuQuery);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhOpenQuery failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   // Add the selected counter to the query
   Status = PdhAddCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time") , NULL, &cpuTotal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhAddCounter failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhCollectQueryData(cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
}

CWindowsSystemCPULoad::~CWindowsSystemCPULoad()
{
   PDH_STATUS Status;

   Status = PdhCloseQuery(cpuQuery);
   
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCloseQuery failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
}

const std::string& CWindowsSystemCPULoad::getDeviceId() const
{
   return m_deviceId;
}

void CWindowsSystemCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   //TODO : A finaliser
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   //context->declareKeyword(m_deviceId, "temp1"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "temp2"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "battery", yApi::CStandardCapacities::BatteryLevel, yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "Rssi"   , yApi::CStandardCapacities::Rssi        , yApi::IYadomsApi::kReadOnly);
}

void CWindowsSystemCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   //TODO : A finaliser
   BOOST_ASSERT_MSG(context, "context must be defined");
   //context->historizeData(m_deviceId, "temp1"  , m_CPULoad);
   //context->historizeData(m_deviceId, "temp2"  , m_temperature2);
   //context->historizeData(m_deviceId, "battery", m_batteryLevel);
   //context->historizeData(m_deviceId, "Rssi"   , m_rssi        );
}

double CWindowsSystemCPULoad::getValue() /*const*/
{
   PDH_FMT_COUNTERVALUE counterVal;
   PDH_STATUS Status;
   DWORD CounterType;

   // Most counters require two sample values to display a formatted value.
   // PDH stores the current sample value and the previously collected
   // sample value. This call retrieves the first value that will be used
   // by PdhGetFormattedCounterValue in the first iteration of the loop
   // Note that this value is lost if the counter does not require two
   // values to compute a displayable value.

   Status = PdhCollectQueryData(cpuQuery);
   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhCollectQueryData failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   Status = PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 | PDH_FMT_NOSCALE, &CounterType, &counterVal);

   if (Status != ERROR_SUCCESS) 
   {
      std::stringstream Message; 
      Message << "PdhGetFormattedCounterValue failed with status:"; 
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   m_CPULoad = counterVal.doubleValue;

   return counterVal.doubleValue;
}


