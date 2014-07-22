#include "stdafx.h"
#include "RaspberryPiTemperatureSensor.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>


CRaspberryPITemperatureSensor::CRaspberryPITemperatureSensor(const std::string & deviceId)
   :m_deviceId(deviceId), m_temperature(25.0)
{
}

CRaspberryPITemperatureSensor::~CRaspberryPITemperatureSensor()
{
}

const std::string& CRaspberryPITemperatureSensor::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CRaspberryPITemperatureSensor::getCapacity() const
{
   return m_Capacity;
}

const std::string& CRaspberryPITemperatureSensor::getKeyword() const
{
   return m_Keyword;
}

void CRaspberryPITemperatureSensor::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{

   // Declare the device
   context->declareDevice(m_deviceId, getModel(), shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceId, "temp1" , yApi::CStandardCapacities::Temperature);
}


void CRaspberryPITemperatureSensor::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "temp1" , m_temperature);
}


double CRaspberryPITemperatureSensor::getValue()
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
      return m_temperature;
   }
   catch(std::exception & ex)
   {
	   YADOMS_LOG(error) << "Fail to read RaspberryPI thermal sensor : " << ex.what();
   }
   catch(...)
   {
	   YADOMS_LOG(error) << "Fail to read RaspberryPI thermal sensor";
   }
   return 0;
}

const std::string& CRaspberryPITemperatureSensor::getModel()
{
   static const std::string model("RaspberryPI temperature sensor");
   return model;
}


