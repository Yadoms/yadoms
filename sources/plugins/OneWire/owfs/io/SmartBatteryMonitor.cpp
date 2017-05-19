#include "stdafx.h"
#include "SmartBatteryMonitor.h"
#include "SingleSwitch.h"
#include "Illumination.hpp"
#include "Humidity.hpp"
#include "Pressure.hpp"
#include "Temperature.hpp"
#include "Voltage.hpp"
#include <shared/Log.h>

namespace owfs
{
   namespace io
   {
      CSmartBatteryMonitor::CSmartBatteryMonitor(const boost::filesystem::path& devicePath)
         :m_sensorTypePath(devicePath / boost::filesystem::path("MultiSensor/type")),
         m_temperatureIo(boost::make_shared<CTemperature>(devicePath)),
         m_humidityIo(boost::make_shared<CHumidity>(devicePath)),
         m_HIH3600Io(boost::make_shared<CHumidity>(devicePath / boost::filesystem::path("HIH3600"))),
         m_HIH4000Io(boost::make_shared<CHumidity>(devicePath / boost::filesystem::path("HIH4000"))),
         m_HTM1735Io(boost::make_shared<CHumidity>(devicePath / boost::filesystem::path("HTM1735"))),
         m_B1_R1_AIo(boost::make_shared<CPressure>(devicePath / boost::filesystem::path("B1-R1-A"))),
         m_S3_R1_AIo(boost::make_shared<CIllumination>(devicePath / boost::filesystem::path("S3-R1-A"))),
         m_vadIo(boost::make_shared<CVoltage>(devicePath, "VAD")),
         m_vddIo(boost::make_shared<CVoltage>(devicePath, "VDD")),
         m_visIo(boost::make_shared<CVoltage>(devicePath, "vis"))
      {
      }

      CSmartBatteryMonitor::~CSmartBatteryMonitor()
      {
      }

      double CSmartBatteryMonitor::readTemperature() const
      {
         return m_temperatureIo->read();
      }

      double CSmartBatteryMonitor::readHIH3600() const
      {
         return m_HIH3600Io->read();
      }

      double CSmartBatteryMonitor::readHumidity() const
      {
         return m_humidityIo->read();
      }

      double CSmartBatteryMonitor::readHIH4000() const
      {
         return m_HIH4000Io->read();
      }

      double CSmartBatteryMonitor::readHTM1735() const
      {
         return m_HTM1735Io->read();
      }

      double CSmartBatteryMonitor::readB1_R1_A() const
      {
         return m_B1_R1_AIo->read();
      }

      double CSmartBatteryMonitor::readS3_R1_A() const
      {
         return m_S3_R1_AIo->read();
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
