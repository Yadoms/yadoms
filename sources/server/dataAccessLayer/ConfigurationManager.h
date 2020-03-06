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
      shared::CDataContainerSharedPtr getServerConfiguration() const override;
      void saveServerConfiguration(const shared::CDataContainer& newConfiguration) override;
      void resetServerConfiguration() override;
      std::string getDatabaseVersion() const override;
      void subscribeOnServerConfigurationChanged(boost::function1<void, shared::CDataContainerSharedPtr > onServerConfigurationChangedFct) override;
      shared::CDataContainerSharedPtr getLocation() const override;
      void saveAutoDetectedLocation(const shared::CDataContainer& newLocation) override;
      shared::CDataContainerSharedPtr getBasicAuthentication() const override;
      // [END] - IConfigurationManager implementation

   protected:
      std::string getConfiguration(const std::string& section) const;
      void saveConfiguration(const std::string& section,
                             const shared::CDataContainer& value) const;

      void notifyServerConfigurationChanged(shared::CDataContainerSharedPtr serverConfiguration);

   private:
      boost::shared_ptr<database::IConfigurationRequester> m_configurationRequester;
      mutable boost::recursive_mutex m_configurationMutex;

      const shared::CDataContainerSharedPtr m_defaultServerConfiguration;

      std::vector<boost::function1<void, shared::CDataContainerSharedPtr>> m_onServerConfigurationChangedObservers;
   };
} //namespace dataAccessLayer 
