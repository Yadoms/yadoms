#include "stdafx.h"
#include "TemperatureSensor.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <fstream>

CTemperatureSensor::CTemperatureSensor(const std::string & deviceId)
   :m_device(deviceId), 
    m_keyword(boost::make_shared<yApi::historization::CTemperature>("Temperature"))
{}

CTemperatureSensor::~CTemperatureSensor()
{}

void CTemperatureSensor::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
   // Declare associated keywords (= values managed by this device)
   if (!api->keywordExists( m_device, m_keyword))
      api->declareKeyword(m_device, m_keyword, details);
}

void CTemperatureSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   BOOST_ASSERT_MSG(api, "api must be defined");

   api->historizeData(m_device, m_keyword);
}

boost::shared_ptr<yApi::historization::IHistorizable> CTemperatureSensor::GetHistorizable() const
{
	return m_keyword;
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
      std::cout << "CPU Temp : " << m_keyword->formatValue() << std::endl;
   }
   catch(std::exception & ex)
   {
      std::cerr << "Fail to read RaspberryPI thermal sensor : " << ex.what() << std::endl;
   }
   catch(...)
   {
      std::cerr << "Fail to read RaspberryPI thermal sensor" << std::endl;
   }
}

