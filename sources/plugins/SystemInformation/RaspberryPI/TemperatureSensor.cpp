#include "stdafx.h"
#include "TemperatureSensor.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <fstream>

CTemperatureSensor::CTemperatureSensor(const std::string & deviceId)
   :m_device(deviceId), 
    m_keyword(new yApi::historization::CTemperature("Temperature"))
{}

CTemperatureSensor::~CTemperatureSensor()
{}

void CTemperatureSensor::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   if (!context->keywordExists( m_device, m_keyword->getKeyword()))
   { 
      context->declareKeyword(m_device, *m_keyword);
   }
}

void CTemperatureSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
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
      YADOMS_LOG(debug) << "CPU Temp : " << m_keyword->formatValue();
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

