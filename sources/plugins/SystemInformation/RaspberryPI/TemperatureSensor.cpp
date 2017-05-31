#include "stdafx.h"
#include "TemperatureSensor.h"
#include <fstream>
#include <shared/Log.h>

CTemperatureSensor::CTemperatureSensor(const std::string & keywordName)
   :m_keyword(boost::make_shared<yApi::historization::CTemperature>(keywordName))
{
}

CTemperatureSensor::~CTemperatureSensor()
{
}

void CTemperatureSensor::read()
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
      m_keyword->set( atof(readValue.c_str()) / 1000.0 );
   }
   catch(std::exception & ex)
   {
      YADOMS_LOG(error) << "Fail to read RaspberryPI thermal sensor : " << ex.what();
   }
   catch(...)
   {
      YADOMS_LOG(error) << "Fail to read RaspberryPI thermal sensor";
   }
}

