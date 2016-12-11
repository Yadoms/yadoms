#include "stdafx.h"
#include "Astronomy.h"
#include <shared/exception/Exception.hpp>
#include <shared/exception/EmptyResult.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
                       IWUConfiguration& wuConfiguration,
                       IdeviceConfiguration& deviceConfiguration)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
     m_deviceName("Astronomy"),
     m_url("http://api.wunderground.com/api/" + wuConfiguration.getAPIKey() + "/astronomy/q/" + m_countryOrState + "/" + m_localisation + ".json"),
     m_moonCharacteristics(boost::make_shared<CMoon>(m_deviceName, "Moon"))
{
   try
   {
      initializeKeywords(api, deviceConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::initializeKeywords(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const IdeviceConfiguration& deviceConfiguration)
{
   if (deviceConfiguration.isAstronomyEnabled())
   {
      // Clear the list
      m_keywords.clear();

      m_keywords.push_back(m_moonCharacteristics->getHistorizable());
      m_moonCharacteristics->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Load.getName(),
                                     shared::plugin::yPluginApi::CStandardCapacities::Load.getUnit());

      // Declare keywords
      std::string m_URL = "www.wunderground.com/";
      api->declareDevice(m_deviceName, m_URL, m_keywords);
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
                                IdeviceConfiguration& deviceConfiguration)
{
   try
   {
      initializeKeywords(api, deviceConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                       const IdeviceConfiguration& deviceConfiguration,
                       const shared::CDataContainer dataToParse)
{
   try
   {
      if (deviceConfiguration.isAstronomyEnabled())
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