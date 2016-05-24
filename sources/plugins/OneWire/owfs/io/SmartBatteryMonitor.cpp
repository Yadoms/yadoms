#include "stdafx.h"
#include "SmartBatteryMonitor.h"
#include "SingleSwitch.h"
#include "Illumination.hpp"
#include "Humidity.hpp"
#include "Pressure.hpp"
#include "Temperature.hpp"
#include "Voltage.hpp"

namespace owfs
{
   namespace io
   {
      CSmartBatteryMonitor::CSmartBatteryMonitor(const boost::filesystem::path& devicePath)
         :m_sensorTypePath(devicePath / boost::filesystem::path("MultiSensor/type")),
          m_temperatureIo(boost::make_shared<CTemperature>(devicePath)),
          m_humidityIo(boost::make_shared<CHumidity>(devicePath)),
          m_pressureIo(boost::make_shared<CPressure>(devicePath)),
          m_light(boost::make_shared<CIllumination>(devicePath)),
          m_vadIo(boost::make_shared<CVoltage>(devicePath, "vad")),
          m_vddIo(boost::make_shared<CVoltage>(devicePath, "vdd")),
          m_visIo(boost::make_shared<CVoltage>(devicePath, "vis"))
      {
      }

      CSmartBatteryMonitor::~CSmartBatteryMonitor()
      {
      }

      ioInterfaces::ISmartBatteryMonitor::ESensorType CSmartBatteryMonitor::readSensorType() const
      {
         std::string readValue = CCommon::read(m_sensorTypePath);

         if (readValue == "MS-T")
            return ioInterfaces::ISmartBatteryMonitor::kMultisensorTemperature;

         if (readValue == "MS-TH")
            return ioInterfaces::ISmartBatteryMonitor::kMultisensorTemperatureHumidity;
   
         if (readValue == "MS-TL")
            return ioInterfaces::ISmartBatteryMonitor::kMultisensorTemperatureLight;

         // Unable to determine sensor type
         return ioInterfaces::ISmartBatteryMonitor::kUnknown;
      }

      double CSmartBatteryMonitor::readTemperature() const
      {
         return m_temperatureIo->read();
      }

      double CSmartBatteryMonitor::readHumidity() const
      {
         return m_humidityIo->read();
      }

      double CSmartBatteryMonitor::readPressure() const
      {
         return m_pressureIo->read();
      }

      double CSmartBatteryMonitor::readLight() const
      {
         return m_light->read();
      }

      double CSmartBatteryMonitor::readVad() const
      {
         return m_vadIo->read();
      }

      double CSmartBatteryMonitor::readVdd() const
      {
         return m_vddIo->read();
      }

      double CSmartBatteryMonitor::readVis() const
      {
         return m_visIo->read();
      }
   }
} // namespace owfs::io