#include "stdafx.h"
#include "RaspberryPITemperatureSensor.h"
#include <shared/Log.h>

CRaspberryPITemperatureSensor::CRaspberryPITemperatureSensor(const std::string & deviceId)
   :m_deviceId(deviceId), m_temperature(25.0)
{
}

CRaspberryPITemperatureSensor::~CRaspberryPITemperatureSensor()
{
}

bool CRaspberryPITemperatureSensor::read()
{
   try
   {
      std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp");
      std::string readValue;
      while(!temperatureFile.eof())
      {
    	  getline(temperatureFile, readValue);
    	  if(!readValue.empty())
    	  {
    		  break;
    	  }
      }
      temperatureFile.close();

      m_temperature = atof(readValue.c_str()) / 1000.0;
      return true;
   }
   catch(std::exception & ex)
   {
	   YADOMS_LOG(error) << "Fail to read RaspberryPI thermal sensor : " << ex.what();
   }
   catch(...)
   {
	   YADOMS_LOG(error) << "Fail to read RaspberryPI thermal sensor";
   }
   return false;
}

const std::string& CRaspberryPITemperatureSensor::getDeviceId() const
{
   return m_deviceId;
}

double CRaspberryPITemperatureSensor::getTemperature() const
{
   return m_temperature;
}


