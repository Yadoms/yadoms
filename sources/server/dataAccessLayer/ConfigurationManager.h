#pragma once
#include "IConfigurationManager.h"
#include "database/IConfigurationRequester.h"

namespace dataAccessLayer
{
   class CConfigurationManager : public IConfigurationManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                                Constructor
      /// \param [in] configurationRequester    The configuration requester
      //--------------------------------------------------------------
      explicit CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester);

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CConfigurationManager();

      // IConfigurationManager implementation
      std::string getSystemConfiguration(const std::string& keyName) const override;
      boost::shared_ptr<const shared::CDataContainer> getSystemConfiguration() override;
      void saveSystemConfiguration(const shared::CDataContainer& newConfiguration) override;
      void resetSystemConfiguration() override;
      void subscribeOnSystemConfigurationChanged(boost::function1<void, boost::shared_ptr<const shared::CDataContainer>> onSystemConfigurationChangedFct) override;
      virtual bool exists(const std::string& section, const std::string& name);
      virtual boost::shared_ptr<database::entities::CConfiguration> getConfiguration(const std::string& section, const std::string& name);
      virtual std::vector<boost::shared_ptr<database::entities::CConfiguration>> getConfigurations(const std::string& section);
      virtual std::vector<boost::shared_ptr<database::entities::CConfiguration>> getConfigurations();
      virtual void updateConfiguration(database::entities::CConfiguration& configurationToUpdate);
      // [END] - IConfigurationManager implementation

   protected:
      //--------------------------------------------------------------
      /// \brief  Post an update configuration notification for the [section,name] data
      //--------------------------------------------------------------
      void notifySystemConfigurationChanged(boost::shared_ptr<const shared::CDataContainer> systemConfiguration);


      static bool isJson(const std::string& str);
      static boost::shared_ptr<shared::CDataContainer> configurationEntitiesToContainer(
         const std::vector<boost::shared_ptr<database::entities::CConfiguration>>& configurationEntities);
      static boost::shared_ptr<std::vector<boost::shared_ptr<database::entities::CConfiguration>>> containerToConfigurationEntities(
         const std::string& sectionName,
         const shared::CDataContainer& container);

   private:
      //--------------------------------------------------------------
      /// \brief           The real data requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IConfigurationRequester> m_configurationRequester;

      const boost::shared_ptr<const shared::CDataContainer> m_defaultSystemConfiguration;

      std::vector<boost::function1<void, boost::shared_ptr<const shared::CDataContainer>>> m_onSystemConfigurationChangedObservers;
   };
} //namespace dataAccessLayer 
