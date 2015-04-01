#include "stdafx.h"
#include "ConfigurationManager.h"
#include <shared/Log.h>

namespace dataAccessLayer {

      CConfigurationManager::CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester, boost::shared_ptr<notification::configurationUpdate::INotifier> notifier)
         :m_configurationRequester(configurationRequester), m_notifier(notifier)
      {
      }
   
      CConfigurationManager::~CConfigurationManager()
      {
      }
    
      void CConfigurationManager::postUpdateNotification(const std::string & section, const std::string & name)
      {
         //post notification
         try
         {
            boost::shared_ptr<database::entities::CConfiguration> notificationData(new database::entities::CConfiguration);
            notificationData->Section = section;
            notificationData->Name = name;
            m_notifier->post(notificationData);
         }
         catch (shared::exception::CException & ex)
         {
            YADOMS_LOG(error) << "Fail to retreive new configuration : " << ex.what();
         }
      }

      void CConfigurationManager::create(database::entities::CConfiguration& configurationToCreate)
      {
         m_configurationRequester->create(configurationToCreate);
         postUpdateNotification(configurationToCreate.Section, configurationToCreate.Name);
      }
      
      bool CConfigurationManager::exists(const std::string & section, const std::string & name)
      {
         return m_configurationRequester->exists(section, name);
      }
      
      boost::shared_ptr<database::entities::CConfiguration> CConfigurationManager::getConfiguration(const std::string & section, const std::string & name)
      {
         return m_configurationRequester->getConfiguration(section, name);
      }
      
      std::vector<boost::shared_ptr<database::entities::CConfiguration> > CConfigurationManager::getConfigurations(const std::string & section)
      {
         return m_configurationRequester->getConfigurations(section);
      }
      
      std::vector<boost::shared_ptr<database::entities::CConfiguration> > CConfigurationManager::getConfigurations()
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
