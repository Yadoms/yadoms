#include "stdafx.h"
#include "Astronomy.h"
#include <shared/exception/Exception.hpp>
#include <shared/exception/EmptyResult.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
                       IWUConfiguration& wuConfiguration,
                       boost::shared_ptr<const shared::ILocation> location,
                       const std::string& deviceName)
   : m_deviceName(deviceName),
     m_url("http://api.wunderground.com/api/" + wuConfiguration.getAPIKey() + "/astronomy/q/" + boost::lexical_cast<std::string>(location->latitude()) + "," + boost::lexical_cast<std::string>(location->longitude()) + ".json"),
     m_moonCharacteristics(boost::make_shared<CMoon>(m_deviceName, "Moon")),
     m_location(location)
{
   try
   {
      initializeKeywords(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::initializeKeywords(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration)
{
   if (wuConfiguration.isAstronomyEnabled())
   {
      // Clear the list
      m_keywords.clear();

      m_keywords.push_back(m_moonCharacteristics->getHistorizable());
      m_moonCharacteristics->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Load().getName(),
                                     shared::plugin::yPluginApi::CStandardCapacities::Load().getUnit());

      // Declare keywords
      std::string m_type = "astronomy";
      shared::CDataContainer details;
      details.set("type", m_type);
      api->declareDevice(m_deviceName, m_type, m_keywords, details);
   }
}

void CAstronomy::onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                IWUConfiguration& wuConfiguration)
{
   try
   {
      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/astronomy/q/" << boost::lexical_cast<std::string>(m_location->latitude()) << "," << boost::lexical_cast<std::string>(m_location->longitude()) << ".json";
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error in Astronomy module :" << e.what() << std::endl;
      throw;
   }
}

void CAstronomy::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                       const shared::CDataContainer dataToParse,
                       IWUConfiguration& wuConfiguration)
{
   try
   {
      if (wuConfiguration.isAstronomyEnabled())
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

// This fonction is not used
void CAstronomy::setCityName(const std::string& CityName)
{}