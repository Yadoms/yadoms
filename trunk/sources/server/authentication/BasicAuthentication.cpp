#include "stdafx.h"
#include "BasicAuthentication.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>
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
      return (!m_skipPasswordCheck && m_isAuthenticationActive);
   }

   bool CBasicAuthentication::authenticate(const std::string & username, const std::string & password) const
   {
      if (isAuthenticationActive())
      {
         boost::lock_guard<boost::mutex> lock(m_configurationMutex);

         try
         {
            return boost::iequals(username, m_currentAuthenticationUsername) && boost::equals(password, m_currentAuthenticationPassword);
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
         boost::shared_ptr<database::entities::CConfiguration> currentConfig = m_configurationManager->getConfiguration(m_configurationSection, m_configurationName);
         
         m_isAuthenticationActive = false;
         m_currentAuthenticationUsername = shared::CStringExtension::EmptyString;
         m_currentAuthenticationPassword = shared::CStringExtension::EmptyString;

         if (currentConfig)
         {
            std::string confValueString = currentConfig->Value();
            if (!confValueString.empty())
            {
               shared::CDataContainer val(confValueString);
               if (val.hasValue(m_configurationActive) && val.get<bool>(m_configurationActive))
               {
                  m_isAuthenticationActive = true;
                  try
                  {
                     if (val.hasValue(m_configurationUser) && val.hasValue(m_configurationPassword))
                     {
                        Poco::URI::decode(val.get<std::string>(m_configurationUser), m_currentAuthenticationUsername);
                        Poco::URI::decode(val.get<std::string>(m_configurationPassword), m_currentAuthenticationPassword);
                     }
                     YADOMS_LOG(warning) << "The configuration system.basic_authentication do not contain 'username' and 'password' settings";
                  }
                  catch (std::exception & ex)
                  {
                     YADOMS_LOG(error) << "Fail to extract configuration data :" << ex.what();
                  }
               }
            }
         }
      }
      catch (std::exception & ex)
      {
         m_isAuthenticationActive = false;
         m_currentAuthenticationUsername = shared::CStringExtension::EmptyString;
         m_currentAuthenticationPassword = shared::CStringExtension::EmptyString;
      }
   }



} //namespace authentication
