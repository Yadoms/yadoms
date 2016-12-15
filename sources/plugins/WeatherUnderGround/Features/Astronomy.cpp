#include "stdafx.h"
#include "Astronomy.h"
#include <shared/exception/Exception.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "../deviceConfiguration.h"

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
                       IWUConfiguration& wuConfiguration,
                       boost::shared_ptr<IdeviceConfiguration> deviceConfiguration,
                       const std::string& deviceName)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
     m_deviceName(deviceName),
     m_url("http://api.wunderground.com/api/" + wuConfiguration.getAPIKey() + "/astronomy/q/" + m_countryOrState + "/" + m_localisation + ".json"),
     m_moonCharacteristics(boost::make_shared<CMoon>(m_deviceName, "Moon")),
     m_deviceConfiguration(deviceConfiguration)
{
   try
   {
      initializeKeywords(api);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::initializeKeywords(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (m_deviceConfiguration->isAstronomyEnabled())
   {
      // Clear the list
      m_keywords.clear();

      m_keywords.push_back(m_moonCharacteristics->getHistorizable());
      m_moonCharacteristics->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Load.getName(),
                                     shared::plugin::yPluginApi::CStandardCapacities::Load.getUnit());

      // Declare keywords
      std::string m_type = "Astronomy";
      shared::CDataContainer details;
      details.set("type", m_type);
      api->declareDevice(m_deviceName, m_type, m_keywords, details);
   }
   else
   {
      try {
         api->removeDevice(m_deviceName);
      }
      catch (std::exception& e)
      {
         std::cout << e.what() << std::endl;
      }
   }
}

void CAstronomy::onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                IWUConfiguration& wuConfiguration)
{
   try
   {
      m_localisation = wuConfiguration.getLocalisation();
      m_countryOrState = wuConfiguration.getCountryOrState();

      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/astronomy/q/" << m_countryOrState << "/" << m_localisation << ".json";
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::onDeviceUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                boost::shared_ptr<IdeviceConfiguration> deviceConfiguration)
{
   try
   {
      m_deviceConfiguration = deviceConfiguration;
      initializeKeywords(api);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                       const shared::CDataContainer dataToParse)
{
   try
   {
      if (m_deviceConfiguration->isAstronomyEnabled())
      {
         m_moonCharacteristics->setParameters(dataToParse,
                                                "moon_phase.percentIlluminated",
                                                "moon_phase.ageOfMoon");
      }

      api->historizeData(m_deviceName, m_keywords);
      std::cout << "Refresh Astronomy Information" << std::endl;
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
      throw;
   }
}

CAstronomy::~CAstronomy()
{}

std::string CAstronomy::getUrl() const
{
   return m_url.str();
}

std::string CAstronomy::getName() const
{
   return m_deviceName;
}