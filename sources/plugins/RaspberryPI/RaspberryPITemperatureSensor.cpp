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

void CRaspberryPITemperatureSensor::read()
{
  FILE *temperatureFile = NULL;
  try
  {
    double T;
    temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    fscanf (temperatureFile, "%lf", &T);
    m_temperature = T / 1000;
  }
  catch(...)
  {
  }

  if(temperatureFile != NULL);
    fclose (temperatureFile);
}

const std::string& CRaspberryPITemperatureSensor::getDeviceId() const
{
   return m_deviceId;
}

double CRaspberryPITemperatureSensor::getTemperature() const
{
   return m_temperature;
}


