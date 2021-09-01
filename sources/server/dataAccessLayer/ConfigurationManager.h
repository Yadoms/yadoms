#pragma once
#include "IConfigurationManager.h"
#include "database/IConfigurationRequester.h"

namespace dataAccessLayer
{
   class CConfigurationManager final : public IConfigurationManager
   {
   public:
      explicit CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester);
      ~CConfigurationManager() override = default;

      // IConfigurationManager implementation
      std::string getExternalConfiguration(const std::string& section) const override;
      void saveExternalConfiguration(const std::string& section,
                                     const std::string& value) override;
      boost::shared_ptr<shared::CDataContainer> getServerConfiguration() const override;
      void saveServerConfiguration(const shared::CDataContainer& newConfiguration) override;
      void resetServerConfiguration() override;
      std::string getDatabaseVersion() const override;
      void subscribeOnServerConfigurationChanged(boost::function1<void, boost::shared_ptr<shared::CDataContainer> > onServerConfigurationChangedFct) override;
      boost::shared_ptr<shared::CDataContainer> getLocation() const override;
      void saveAutoDetectedLocation(const shared::CDataContainer& newLocation) override;
      boost::shared_ptr<shared::CDataContainer> getBasicAuthentication() const override;
      // [END] - IConfigurationManager implementation

   private:
      static bool quiteEqual(double a, double b);

      std::string getConfiguration(const std::string& section) const;
      void saveConfiguration(const std::string& section,
                             const std::string& value) const;

      void notifyServerConfigurationChanged(const boost::shared_ptr<shared::CDataContainer>& serverConfiguration);

      boost::shared_ptr<database::IConfigurationRequester> m_configurationRequester;
      mutable boost::recursive_mutex m_configurationMutex;

      const boost::shared_ptr<shared::CDataContainer> m_defaultServerConfiguration;

      std::vector<boost::function1<void, boost::shared_ptr<shared::CDataContainer>>> m_onServerConfigurationChangedObservers;
   };
} //namespace dataAccessLayer 
