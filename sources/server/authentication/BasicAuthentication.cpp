#include "stdafx.h"
#include "BasicAuthentication.h"
#include <shared/Log.h>
#include <Poco/MD5Engine.h>


#include "notification/Helpers.hpp"

namespace authentication {

   const std::string CBasicAuthentication::m_configurationSection("system");
   const std::string CBasicAuthentication::m_configurationName("basicAuthentication");
   const std::string CBasicAuthentication::m_configurationActive("active");
   const std::string CBasicAuthentication::m_configurationUser("user");
   const std::string CBasicAuthentication::m_configurationPassword("password");

   CBasicAuthentication::CBasicAuthentication(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager, bool skipPasswordCheck)
      :m_configurationManager(configurationManager), m_skipPasswordCheck(skipPasswordCheck)
   {
      if (!m_skipPasswordCheck)
      {
         //read configuration from database
         updateConfiguration();

         m_observer = notification::CHelpers::subscribeBasicObserver<database::entities::CConfiguration>(boost::bind(&CBasicAuthentication::onConfigurationUpdated, this, _1));
      }
   }

   CBasicAuthentication::~CBasicAuthentication()
   {
      if (!m_skipPasswordCheck && m_observer)
      {
         notification::CHelpers::unsubscribeObserver(m_observer);
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
            Poco::MD5Engine md5;
            md5.update(password);
            std::string cypherPassword(Poco::DigestEngine::digestToHex(md5.digest()));

            return boost::iequals(username, m_currentAuthenticationUsername) && boost::equals(cypherPassword, m_currentAuthenticationPassword);
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(error) << "Fail to read configuration value :" << ex.what();
         }
      }
      return true;
   }

   void CBasicAuthentication::onConfigurationUpdated(boost::shared_ptr<database::entities::CConfiguration> newConfiguration)
   {
      if (newConfiguration && boost::iequals(newConfiguration->Section(), m_configurationSection) && boost::iequals(newConfiguration->Name(), m_configurationName))
      {
         YADOMS_LOG(information) << "Authentication settings have changes, reload them";
         updateConfiguration();
      }
   }
   
   void CBasicAuthentication::updateConfiguration()
   {
      boost::lock_guard<boost::mutex> lock(m_configurationMutex);
      try
      {
         boost::shared_ptr<database::entities::CConfiguration> currentConfig = m_configurationManager->getConfiguration(m_configurationSection, m_configurationName);
         
         m_isAuthenticationActive = false;
         m_currentAuthenticationUsername = std::string();
         m_currentAuthenticationPassword = std::string();

         if (currentConfig)
         {
            std::string confValueString = currentConfig->Value();
            if (!confValueString.empty())
            {
               shared::CDataContainer val(confValueString);
               if (val.exists(m_configurationActive) && val.get<bool>(m_configurationActive))
               {
                  m_isAuthenticationActive = true;
                  try
                  {
                     if (val.exists(m_configurationUser) && val.exists(m_configurationPassword))
                     {
                        m_currentAuthenticationUsername = val.get<std::string>(m_configurationUser);
                        m_currentAuthenticationPassword = val.get<std::string>(m_configurationPassword);
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "The configuration system.basicAuthentication do not contain 'user' and 'password' settings";
                     }
                  }
                  catch (std::exception & ex)
                  {
                     YADOMS_LOG(error) << "Fail to extract configuration data :" << ex.what();
                  }
               }
            }
         }
      }
      catch (std::exception &)
      {
         YADOMS_LOG(warning) << "Can not find configuration item : system.basicAuthentication, disable authentication";
         m_isAuthenticationActive = false;
         m_currentAuthenticationUsername = std::string();
         m_currentAuthenticationPassword = std::string();
      }
   }



} //namespace authentication
