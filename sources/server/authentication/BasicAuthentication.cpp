#include "stdafx.h"
#include "BasicAuthentication.h"
#include <shared/Log.h>
#include "notifications/ConfigurationUpdatedNotification.h"
#include <Poco/URI.h>

namespace authentication {

   const std::string CBasicAuthentication::m_configurationSection("system");
   const std::string CBasicAuthentication::m_configurationName("basicAuthentication");
   const std::string CBasicAuthentication::m_configurationActive("active");
   const std::string CBasicAuthentication::m_configurationUser("user");
   const std::string CBasicAuthentication::m_configurationPassword("password");

   CBasicAuthentication::CBasicAuthentication(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager, boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, bool skipPasswordCheck)
      :m_configurationManager(configurationManager), m_notificationCenter(notificationCenter), m_skipPasswordCheck(skipPasswordCheck), m_monitorConfigurationUpdates(this, &CBasicAuthentication::monitorConfigurationUpdate)
   {
      if (!m_skipPasswordCheck)
      {
         //read configuration from database
         updateConfiguration();

         //start the configuration updates monitoring
         m_monitorConfigurationUpdates.start();
      }
   }

   CBasicAuthentication::~CBasicAuthentication()
   {
      if (!m_skipPasswordCheck)
      {
         m_monitorConfigurationUpdates.stop(); // request stop
         m_monitorConfigurationUpdates.wait(); // wait until activity actually stops
      }
   }

   bool CBasicAuthentication::isAuthenticationActive() const
   {
      if (!m_skipPasswordCheck)
      {
         boost::lock_guard<boost::mutex> lock(m_configurationMutex);

         try
         {
            if (m_currentConfiguration)
            {
               std::string confValueString = m_currentConfiguration->Value();
               if (!confValueString.empty())
               {
                  shared::CDataContainer val(confValueString);
                  return (val.hasValue(m_configurationActive) && val.get<bool>(m_configurationActive));
               }
               else
               {
                  //not confiugre
               }
            }
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(error) << "Fail to read configuration value :" << ex.what();
         }
      }
      //in all cases (not configured, not activated, passwordSkip) just return false
      return false;
   }

   bool CBasicAuthentication::authenticate(const std::string & username, const std::string & password) const
   {
      if (isAuthenticationActive())
      {
         boost::lock_guard<boost::mutex> lock(m_configurationMutex);

         try
         {
            if (m_currentConfiguration)
            {
               std::string confValueString = m_currentConfiguration->Value();
               if (!confValueString.empty())
               {
                  shared::CDataContainer val(confValueString);
                  if (val.hasValue(m_configurationActive) && val.get<bool>(m_configurationActive))
                  {
                     try
                     {
                        if (val.hasValue(m_configurationUser) && val.hasValue(m_configurationPassword))
                        {
                           std::string configUsername, configPassword;
                           Poco::URI::decode(val.get<std::string>(m_configurationUser), configUsername);
                           Poco::URI::decode(val.get<std::string>(m_configurationPassword), configPassword);
                           return boost::iequals(username, configUsername) && boost::equals(password, configPassword);
                        }
                        YADOMS_LOG(warning) << "The configuration system.basic_authentication do not contain 'username' and 'password' settings";
                        return false;
                     }
                     catch (std::exception & ex)
                     {
                        YADOMS_LOG(error) << "Fail to extract configuration data :" << ex.what();
                        return false;
                     }
                  }
                  else
                  {
                     YADOMS_LOG(warning) << "The configuration system.basic_authentication do not contain the 'activated' setting";
                  }
               }
               else
               {
                  //not confiugre
               }
            }
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(error) << "Fail to read configuration value :" << ex.what();
         }
      }
      return true;
   }

   void CBasicAuthentication::monitorConfigurationUpdate()
   {
      BOOST_ASSERT_MSG(m_notificationCenter, "Notification center must be defined");

      try
      {
         m_notificationCenter->registerObserver(this);

         while (!m_monitorConfigurationUpdates.isStopped())
         {
            switch (m_notificationCenter->waitForNotifications(this, boost::posix_time::milliseconds(300)))
            {
            case shared::notification::CNotificationCenter::kNotification:
               //check if notification is a newAcquisition
               if (m_notificationCenter->isNotificationTypeOf< boost::shared_ptr<notifications::CConfigurationUpdatedNotification> >(this))
               {
                  YADOMS_LOG(information) << "Authentication settings have changes, reload them";
                  boost::shared_ptr<notifications::CConfigurationUpdatedNotification> configurationUpdated = m_notificationCenter->getNotificationData< boost::shared_ptr<notifications::CConfigurationUpdatedNotification> >(this);
                  if (boost::iequals(configurationUpdated->getSection(), m_configurationSection) && boost::iequals(configurationUpdated->getName(), m_configurationName))
                  {
                     updateConfiguration();
                  }
               }
               break;
            }
         }
      }
      catch (std::exception &ex)
      {
         YADOMS_LOG(error) << "Fail to monitor configuration updates :" << ex.what();
      }
      m_notificationCenter->unregisterObserver(this);

   }


   void CBasicAuthentication::updateConfiguration()
   {
      boost::lock_guard<boost::mutex> lock(m_configurationMutex);
      try
      {
         m_currentConfiguration = m_configurationManager->getConfiguration(m_configurationSection, m_configurationName);
      }
      catch (std::exception & ex)
      {
         m_currentConfiguration = boost::shared_ptr<database::entities::CConfiguration>();
      }
   }



} //namespace authentication
