#include "stdafx.h"
#include "Astronomy.h"
#include "ErrorAnswerHandler.h"
#include <shared/exception/Exception.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
                       IWUConfiguration& wuConfiguration,
                       const std::string& deviceName,
                       const std::string& prefix)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
   m_deviceName(deviceName),
     m_moonCharacteristics(boost::make_shared<CMoon>(deviceName, prefix + "Moon"))
{
   m_catchError = false;
   m_url.str("");
   m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/astronomy/q/" << m_countryOrState << "/" << m_localisation << ".json";

   try
   {
      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
      m_catchError = true;
   }
}

void CAstronomy::initializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                                     IWUConfiguration& wuConfiguration)
{
   // Clear the list
   m_keywords.clear();

   if (wuConfiguration.IsAstronomyEnabled())
   {
      m_keywords.push_back(m_moonCharacteristics->getHistorizable());
      m_moonCharacteristics->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Load.getName(),
                                     shared::plugin::yPluginApi::CStandardCapacities::Load.getUnit());
   }

   // Declare keywords
   std::string m_URL = "www.wunderground.com/";
   api->declareDevice(m_deviceName, m_URL, m_keywords);
}

void CAstronomy::onUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                          IWUConfiguration& wuConfiguration)
{
   try
   {
      m_localisation = wuConfiguration.getLocalisation();

      //read the country or State code
      m_countryOrState = wuConfiguration.getCountryOrState();

      m_url.str("");

      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/astronomy/q/" << m_countryOrState << "/" << m_localisation << ".json";

      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
      m_catchError = true;
   }
}

bool CAstronomy::request(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      m_catchError = false;
      m_data = m_webServer.SendGetRequest(m_url.str());
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Astronomy :" << e.what() << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
      m_catchError = true;
   }

   return m_catchError;
}

void CAstronomy::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                       const IWUConfiguration& wuConfiguration)
{
   try
   {
      ErrorAnswerHandler Response(api, m_data);
      m_catchError = Response.ContainError();

      if (!m_catchError)
      {
         if (wuConfiguration.IsAstronomyEnabled())
         {
            m_moonCharacteristics->setParameters(m_data,
                                                 "moon_phase.percentIlluminated",
                                                 "moon_phase.ageOfMoon");
         }

         api->historizeData(m_deviceName, m_keywords);
      }
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
   }
}

bool CAstronomy::isModuleInFault() const
{
   return m_catchError;
}

CAstronomy::~CAstronomy()
{
}

