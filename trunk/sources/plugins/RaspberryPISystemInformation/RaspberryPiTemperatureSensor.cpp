#include "stdafx.h"
#include "RaspberryPiTemperatureSensor.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>


CRaspberryPiTemperatureSensor::CRaspberryPiTemperatureSensor(const std::string & deviceId)
   :m_device(deviceId), m_keyword("Temp")
{
}

CRaspberryPiTemperatureSensor::~CRaspberryPiTemperatureSensor()
{
}

void CRaspberryPiTemperatureSensor::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_keyword);
}

void CRaspberryPiTemperatureSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}


void CRaspberryPiTemperatureSensor::read()
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

      m_keyword.set( atof(readValue.c_str()) / 1000.0 );
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

