#include "stdafx.h"
#include "RaspberryPITemperatureSensor.h"
#include <shared/Log.h>

CRaspberryPITemperatureSensor::CRaspberryPITemperatureSensor(const std::string & deviceId)
   :m_deviceId(deviceId), m_temperature(25.0)//TODO : prévoir que la température ne puisse pas être remontée (erreur d'accès au fichier par exemple). Dans ce cas, il ne faut rien remonter.
{
}

CRaspberryPITemperatureSensor::~CRaspberryPITemperatureSensor()
{
}

void CRaspberryPITemperatureSensor::read()
{
   try
   {
      std::ifstream temperatureFile("/sys/class/thermal/thermal_zone0/temp");
      std::string readValue;
      while(!temperatureFile.eof())
         getline(temperatureFile, readValue);

      m_temperature = atof(readValue.c_str()) / 1000;
   }
   catch(...)
   {
   }
}

const std::string& CRaspberryPITemperatureSensor::getDeviceId() const
{
   return m_deviceId;
}

double CRaspberryPITemperatureSensor::getTemperature() const
{
   return m_temperature;
}


