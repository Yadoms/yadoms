#include "stdafx.h"
#include "ConfigurationManager.h"
#include <shared/exception/EmptyResult.hpp>

namespace dataAccessLayer
{
   class CDefaultServerConfiguration
   {
   public:
      boost::shared_ptr<shared::CDataContainer> get() const
      {
         return m_configuration;
      }

      CDefaultServerConfiguration()
         : m_configuration(shared::CDataContainer::make())
      {
         m_configuration->set("firstStart", true);

         // Accepted values for location status :
         //   - "default" : when location comes from the default configuration (this one)
         //   - "autoDetected" : when location was automatically detected (ie from IP)
         //   - "userDefined" : when location was defined by user
         m_configuration->set("location.status", "default");
         m_configuration->set("location.latitude", 48.853);
         m_configuration->set("location.longitude", 2.35);
         m_configuration->set("location.altitude", 0.0);
         m_configuration->set("location.timezone", "Europe/Paris");

         m_configuration->set("basicAuthentication.active", false);
         m_configuration->set("basicAuthentication.user", "admin");
         m_configuration->set("basicAuthentication.password", std::string());
      }

      virtual ~CDefaultServerConfiguration() = default;

   private:
      const boost::shared_ptr<shared::CDataContainer> m_configuration;
   };

   CConfigurationManager::CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester)
      : m_configurationRequester(configurationRequester),
        m_defaultServerConfiguration(CDefaultServerConfiguration().get())
   {
   }

   std::string CConfigurationManager::getExternalConfiguration(const std::string& section) const
   {
      return getConfiguration("external." + section);
   }

   void CConfigurationManager::saveExternalConfiguration(const std::string& section,
                                                         const shared::CDataContainer& value)
   {
      saveConfiguration("external." + section,
                        value);
   }

   void CConfigurationManager::notifyServerConfigurationChanged(boost::shared_ptr<shared::CDataContainer> serverConfiguration)
   {
      for (const auto& fct : m_onServerConfigurationChangedObservers)
         fct(serverConfiguration);
   }

   boost::shared_ptr<shared::CDataContainer> CConfigurationManager::getServerConfiguration() const
   {
      try
      {
         // Returned configuration is the default one, overriden by stored values
         auto configuration = m_defaultServerConfiguration->copy();
         const shared::CDataContainer cfg(getConfiguration("server"));
         configuration->mergeFrom(cfg);
         return configuration;
      }
      catch (shared::exception::CEmptyResult&)
      {
         return m_defaultServerConfiguration;
      }
   }

   void CConfigurationManager::saveServerConfiguration(const shared::CDataContainer& newConfiguration)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);

      // If location changed, mark it as user-modified
      const auto currentConfiguration = getServerConfiguration();
      auto configurationToSave = newConfiguration.copy();
      if (newConfiguration.get<double>("location.latitude") != currentConfiguration->get<double>("location.latitude") ||
         newConfiguration.get<double>("location.longitude") != currentConfiguration->get<double>("location.longitude") ||
         newConfiguration.get<double>("location.altitude") != currentConfiguration->get<double>("location.altitude") ||
         newConfiguration.get<std::string>("location.timezone") != currentConfiguration->get<std::string>("location.timezone"))
      {
         configurationToSave->set("location.status", "userDefined");
      }

      saveConfiguration("server",*configurationToSave.get());
      notifyServerConfigurationChanged(getServerConfiguration());
   }

   void CConfigurationManager::resetServerConfiguration()
   {
      auto resetConfiguration = m_defaultServerConfiguration->copy();
      // Reset configuration must not overwrite firstStart flag
      resetConfiguration->set("firstStart", getServerConfiguration()->get<bool>("firstStart"));
      saveServerConfiguration(*resetConfiguration.get());
   }

   std::string CConfigurationManager::getDatabaseVersion() const
   {
      return getConfiguration("databaseVersion");
   }

   void CConfigurationManager::subscribeOnServerConfigurationChanged(boost::function1<void, boost::shared_ptr<shared::CDataContainer>> onServerConfigurationChangedFct)
   {
      m_onServerConfigurationChangedObservers.push_back(onServerConfigurationChangedFct);
   }

   boost::shared_ptr<shared::CDataContainer> CConfigurationManager::getLocation() const
   {
      return getServerConfiguration()->get<boost::shared_ptr<shared::CDataContainer>>("location");
   }

   void CConfigurationManager::saveAutoDetectedLocation(const shared::CDataContainer& newLocation)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);
      auto serverConfiguration = getServerConfiguration()->copy();

      // Overwrite location only if not set by user
      if (serverConfiguration->get<std::string>("location.status") == "userDefined")
         return;
      
      serverConfiguration->set("location.status", "autoDetected");
      serverConfiguration->set("location.latitude", newLocation.get<double>("latitude"));
      serverConfiguration->set("location.longitude", newLocation.get<double>("longitude"));
      serverConfiguration->set("location.altitude", newLocation.get<double>("altitude"));
      serverConfiguration->set("location.timezone", newLocation.get<std::string>("timezone"));

      saveConfiguration("server", *serverConfiguration.get());
      notifyServerConfigurationChanged(getServerConfiguration());
   }

   boost::shared_ptr<shared::CDataContainer> CConfigurationManager::getBasicAuthentication() const
   {
      return getServerConfiguration()->get<boost::shared_ptr<shared::CDataContainer>>("basicAuthentication");
   }

   std::string CConfigurationManager::getConfiguration(const std::string& section) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);
      return m_configurationRequester->getConfiguration(section)->Value();
   }

   void CConfigurationManager::saveConfiguration(const std::string& section,
                                                 const shared::CDataContainer& value) const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_configurationMutex);
      m_configurationRequester->updateConfiguration(section,
                                                    value.serialize());
   }
} //namespace dataAccessLayer 
