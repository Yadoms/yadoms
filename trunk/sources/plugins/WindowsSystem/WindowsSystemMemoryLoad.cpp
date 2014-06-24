#include "stdafx.h"
#include "WindowsSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>

CWindowsSystemMemoryLoad::CWindowsSystemMemoryLoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_memoryLoad(0)
{}

CWindowsSystemMemoryLoad::~CWindowsSystemMemoryLoad()
{}

const std::string& CWindowsSystemMemoryLoad::getDeviceId() const
{
   return m_deviceId;
}

void CWindowsSystemMemoryLoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   //TODO: A finaliser
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceId, "temp1"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "temp2"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "battery", yApi::CStandardCapacities::BatteryLevel, yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "Rssi"   , yApi::CStandardCapacities::Rssi        , yApi::IYadomsApi::kReadOnly);
}

void CWindowsSystemMemoryLoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   //TODO: A finaliser
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "temp1"  , m_memoryLoad);
   //context->historizeData(m_deviceId, "temp2"  , m_temperature2);
   //context->historizeData(m_deviceId, "battery", m_batteryLevel);
   //context->historizeData(m_deviceId, "Rssi"   , m_rssi        );
}

double CWindowsSystemMemoryLoad::getValue()
{
   MEMORYSTATUSEX statex;

   statex.dwLength = sizeof (statex);

   if (GlobalMemoryStatusEx (&statex))
      m_memoryLoad = (double(statex.ullTotalPhys - statex.ullAvailPhys)*100 / statex.ullTotalPhys );
   else
   {
      std::stringstream Message;
      Message << "Fail to read Windows system memory size :";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   return m_memoryLoad;
}


