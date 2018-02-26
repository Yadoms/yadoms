#pragma once
#include "IConfigurationManager.h"
#include "database/IConfigurationRequester.h"

namespace dataAccessLayer
{
   class CConfigurationManager : public IConfigurationManager
   {
   public:
      explicit CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester);
      virtual ~CConfigurationManager();

      // IConfigurationManager implementation
      std::string getExternalConfiguration(const std::string& section) const override;
      void saveExternalConfiguration(const std::string& section,
                                     const shared::CDataContainer& value) override;
      boost::shared_ptr<const shared::CDataContainer> getServerConfiguration() const override;
      void saveServerConfiguration(const shared::CDataContainer& newConfiguration) override;
      void resetServerConfiguration() override;
      std::string getDatabaseVersion() const override;
      void subscribeOnServerConfigurationChanged(
         boost::function1<void, boost::shared_ptr<const shared::CDataContainer>> onServerConfigurationChangedFct) override;
      shared::CDataContainer CConfigurationManager::getLocation() const override;
      void saveLocation(const shared::CDataContainer& newLocation) override;
      shared::CDataContainer getBasicAuthentication() const override;
      void saveBasicAuthentication(const shared::CDataContainer& newBasicAuthentication) override;
      // [END] - IConfigurationManager implementation

   protected:
      std::string getConfiguration(const std::string& section) const;
      void saveConfiguration(const std::string& section,
                             const shared::CDataContainer& value) const;

      void notifyServerConfigurationChanged(boost::shared_ptr<const shared::CDataContainer> serverConfiguration);

   private:
      boost::shared_ptr<database::IConfigurationRequester> m_configurationRequester;
      mutable boost::recursive_mutex m_configurationMutex;

      const boost::shared_ptr<const shared::CDataContainer> m_defaultServerConfiguration;

      std::vector<boost::function1<void, boost::shared_ptr<const shared::CDataContainer>>> m_onServerConfigurationChangedObservers;
   };
} //namespace dataAccessLayer 
