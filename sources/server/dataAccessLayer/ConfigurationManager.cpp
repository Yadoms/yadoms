#include "stdafx.h"
#include "ConfigurationManager.h"
#include <shared/Log.h>
#include "notification/Helpers.hpp"
#include <shared/exception/EmptyResult.hpp>

namespace dataAccessLayer
{
   class CDefaultSystemConfiguration
   {
   public:
      boost::shared_ptr<shared::CDataContainer> get() const
      {
         return m_configuration;
      }

      CDefaultSystemConfiguration::CDefaultSystemConfiguration()
         : m_configuration(boost::make_shared<shared::CDataContainer>())
      {
         m_configuration->set("developerMode", false);

         m_configuration->set("location.latitude", "48.853");
         m_configuration->set("location.longitude", "2.35");
         m_configuration->set("location.altitude", "0.0");
         m_configuration->set("location.timezone", "Europe/Paris");

         m_configuration->set("language", "en");

         m_configuration->set("advancedParameters", false);
         m_configuration->set("dateFormatString", "LLL");
         m_configuration->set("refreshPage", false);

         m_configuration->set("basicAuthentication.active", false);
         m_configuration->set("basicAuthentication.user", "admin");
         m_configuration->set("basicAuthentication.password", std::string());
      }

      CDefaultSystemConfiguration::~CDefaultSystemConfiguration()
      {
      }

   private:
      const boost::shared_ptr<shared::CDataContainer> m_configuration;
   };

   CConfigurationManager::CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester)
      : m_configurationRequester(configurationRequester),
        m_defaultSystemConfiguration(CDefaultSystemConfiguration().get())
   {
   }

   CConfigurationManager::~CConfigurationManager()
   {
   }

   void CConfigurationManager::postUpdateNotification(const std::string& section, const std::string& name)
   {
      //post notification
      try
      {
         boost::shared_ptr<database::entities::CConfiguration> notificationData(new database::entities::CConfiguration);
         notificationData->Section = section;
         notificationData->Name = name;

         notification::CHelpers::postChangeNotification(notificationData, notification::change::EChangeType::kUpdate);
      }
      catch (shared::exception::CException& ex)
      {
         YADOMS_LOG(error) << "Fail to retreive new configuration : " << ex.what();
      }
   }

   std::string CConfigurationManager::getSystemConfiguration(const std::string& keyName) const
   {
      try
      {
         return m_configurationRequester->getConfiguration("system", keyName)->Value();
      }
      catch (shared::exception::CEmptyResult&)
      {
         return m_defaultSystemConfiguration->get<std::string>(keyName);
      }
   }

   boost::shared_ptr<const shared::CDataContainer> CConfigurationManager::getSystemConfiguration()
   {
      try
      {
         // Returned configuration is the default one, overided by stored values
         auto configuration = boost::make_shared<shared::CDataContainer>(*m_defaultSystemConfiguration);
         configuration->mergeFrom(*configurationEntitiesToContainer(m_configurationRequester->getConfigurations("system")));
         return configuration;
      }
      catch (shared::exception::CEmptyResult&)
      {
         return m_defaultSystemConfiguration;
      }
   }

   void CConfigurationManager::saveSystemConfiguration(const shared::CDataContainer& newConfiguration)
   {
      const auto& c = containerToConfigurationEntities("system", newConfiguration);
      for (const auto& entity : *c)
         m_configurationRequester->updateConfiguration(*entity);
   }

   bool CConfigurationManager::isJson(const std::string& str)
   {
      static const boost::regex JsonPattern("^\\{.*\\}$");
      return boost::regex_match(str, JsonPattern);
   }

   boost::shared_ptr<shared::CDataContainer> CConfigurationManager::configurationEntitiesToContainer(
      const std::vector<boost::shared_ptr<database::entities::CConfiguration>>& configurationEntities)
   {
      auto container = boost::make_shared<shared::CDataContainer>();

      for (const auto& entity : configurationEntities)
         if (isJson(entity->Value()))
            container->set(entity->Name(), shared::CDataContainer(entity->Value()));
         else
            container->set(entity->Name(), entity->Value());

      return container;
   }

   boost::shared_ptr<std::vector<boost::shared_ptr<database::entities::CConfiguration>>> CConfigurationManager::containerToConfigurationEntities(
      const std::string& sectionName,
      const shared::CDataContainer& container)
   {
      auto configurationEntities = boost::make_shared<std::vector<boost::shared_ptr<database::entities::CConfiguration>>>();

      for (const auto& key : container.getKeys())
      {
         auto item = boost::make_shared<database::entities::CConfiguration>();
         item->Section = sectionName;
         item->Name = key;
         item->Value = container.containsValue(key) ? container.get<std::string>(key) : container.get<shared::CDataContainer>(key).serialize();
         configurationEntities->push_back(item);
      }

      return configurationEntities;
   }

   void CConfigurationManager::resetSystemConfiguration()
   {
      try
      {
         m_configurationRequester->removeConfigurations("system");
      }
      catch (shared::exception::CEmptyResult&)
      {
      }

      for (const auto& key : m_defaultSystemConfiguration->getKeys())
      {
         database::entities::CConfiguration configuration;
         configuration.Section = "system";
         configuration.Name = key;
         configuration.Value = m_defaultSystemConfiguration->get(key);
         m_configurationRequester->create(configuration);
      }
   }

   bool CConfigurationManager::exists(const std::string& section, const std::string& name)
   {
      return m_configurationRequester->exists(section, name);
   }

   boost::shared_ptr<database::entities::CConfiguration> CConfigurationManager::getConfiguration(const std::string& section, const std::string& name)
   {
      return m_configurationRequester->getConfiguration(section, name);
   }

   std::vector<boost::shared_ptr<database::entities::CConfiguration>> CConfigurationManager::getConfigurations(const std::string& section)
   {
      return m_configurationRequester->getConfigurations(section);
   }

   std::vector<boost::shared_ptr<database::entities::CConfiguration>> CConfigurationManager::getConfigurations()
   {
      return m_configurationRequester->getConfigurations();
   }

   void CConfigurationManager::updateConfiguration(database::entities::CConfiguration& configurationToUpdate)
   {
      m_configurationRequester->updateConfiguration(configurationToUpdate);
      postUpdateNotification(configurationToUpdate.Section, configurationToUpdate.Name);
   }

   void CConfigurationManager::removeConfiguration(database::entities::CConfiguration& configurationToRemove)
   {
      m_configurationRequester->removeConfiguration(configurationToRemove);
      postUpdateNotification(configurationToRemove.Section, configurationToRemove.Name);
   }
} //namespace dataAccessLayer 
