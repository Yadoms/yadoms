#pragma once
#include "IAuthentication.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include <shared/notification/NotificationCenter.h>
#include "database/entities/Entities.h"
#include "Poco/Activity.h"

namespace authentication {

   //-------------------------------------
   ///\brief Class which handle basic authentication (single user/password check)
   //-------------------------------------
   class CBasicAuthentication : public IAuthentication
   {
   public:
      //-------------------------------------
      ///\brief Constructor
      ///\param [in]    configurationManager    Configuration manager
      ///\param [in]    notificationCenter      Notification center
      ///\param [in]    m_skipPasswordCheck     If true the password will never be checked
      //-------------------------------------
      CBasicAuthentication(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager, boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, bool skipPasswordCheck);
      
      //-------------------------------------
      ///\brief Destructor
      //-------------------------------------
      virtual ~CBasicAuthentication();

      // IAuthentifier implementation
      virtual bool authenticate(const std::string & username, const std::string & password) const;
      // [END] IAuthentifier implementation

      
   private:
      //--------------------------------------------------------------
      /// \brief  Function which wait for configration updates
      //--------------------------------------------------------------
      void monitorConfigurationUpdate();

      //--------------------------------------------------------------
      /// \brief  Function which read authentication settings from database
      //--------------------------------------------------------------
      void updateConfiguration();
   private:
      //--------------------------------------------------------------
      /// \brief           The configuration manager
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;

      //--------------------------------------------------------------
      /// \brief           The notification center
      //--------------------------------------------------------------
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;

      //--------------------------------------------------------------
      /// \brief           The current configuration for basic authentication
      //--------------------------------------------------------------
      boost::shared_ptr<database::entities::CConfiguration> m_currentConfiguration;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the m_currentConfiguration
      //--------------------------------------------------------------
      mutable boost::mutex m_configurationMutex;

      //--------------------------------------------------------------
      /// \brief	   Thread which check for configuration updates
      //--------------------------------------------------------------
      Poco::Activity<CBasicAuthentication> m_monitorConfigurationUpdates;

      //--------------------------------------------------------------
      /// \brief	   Indicate if password check is skipped for current yadoms instance
      //--------------------------------------------------------------
      bool m_skipPasswordCheck;

      //--------------------------------------------------------------
      /// \brief	   The section used to store authentication settings
      //--------------------------------------------------------------
      static const std::string m_configurationSection;
      //--------------------------------------------------------------
      /// \brief	   The name used to store authentication settings
      //--------------------------------------------------------------
      static const std::string m_configurationName;
      //--------------------------------------------------------------
      /// \brief	   The active setting nmae in authentication settings
      //--------------------------------------------------------------
      static const std::string m_configurationActive;
      //--------------------------------------------------------------
      /// \brief	   The username setting nmae in authentication settings
      //--------------------------------------------------------------
      static const std::string m_configurationUser;
      //--------------------------------------------------------------
      /// \brief	   The password setting nmae in authentication settings
      //--------------------------------------------------------------
      static const std::string m_configurationPassword;
   };

} //namespace authentication
