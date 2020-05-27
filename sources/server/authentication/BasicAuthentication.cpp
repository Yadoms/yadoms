#include "stdafx.h"
#include "BasicAuthentication.h"
#include <shared/Log.h>
#include <Poco/MD5Engine.h>


#include "notification/Helpers.hpp"

namespace authentication
{
   CBasicAuthentication::CBasicAuthentication(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                                              bool skipPasswordCheck)
      : m_configurationManager(configurationManager), m_skipPasswordCheck(skipPasswordCheck)
   {
      if (!m_skipPasswordCheck)
      {
         updateConfiguration();

         configurationManager->subscribeOnServerConfigurationChanged([&](boost::shared_ptr<const shared::CDataContainer> serverConfiguration)
         {
            updateConfiguration();
         });
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

   bool CBasicAuthentication::authenticate(const std::string& username, const std::string& password) const
   {
      if (isAuthenticationActive())
      {
         boost::lock_guard<boost::mutex> lock(m_configurationMutex);

         try
         {
            Poco::MD5Engine md5;
            md5.update(password);
            const auto cypherPassword(Poco::DigestEngine::digestToHex(md5.digest()));

            return boost::iequals(username, m_currentAuthenticationUsername) && boost::equals(cypherPassword, m_currentAuthenticationPassword);
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to read configuration value :" << ex.what();
         }
      }
      return true;
   }

   void CBasicAuthentication::updateConfiguration()
   {
      try
      {
         boost::lock_guard<boost::mutex> lock(m_configurationMutex);
         const auto basicAuthenConfiguration(m_configurationManager->getBasicAuthentication());
         m_isAuthenticationActive = basicAuthenConfiguration->getWithDefault<bool>("active", false);
         m_currentAuthenticationUsername = basicAuthenConfiguration->get<std::string>("user");
         m_currentAuthenticationPassword = basicAuthenConfiguration->get<std::string>("password");
      }
      catch (std::exception& ex)
      {
         YADOMS_LOG(error) << "Fail to extract configuration data :" << ex.what();
         m_isAuthenticationActive = false;
         m_currentAuthenticationUsername.clear();
         m_currentAuthenticationPassword.clear();
      }
   }
} //namespace authentication
