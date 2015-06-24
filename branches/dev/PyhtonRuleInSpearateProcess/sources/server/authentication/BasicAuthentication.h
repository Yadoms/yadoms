#pragma once
#include "IAuthentication.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "notification/IObserver.h"
#include "database/entities/Entities.h"


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
      ///\param [in]    notifier                Notifier for configuration updates
      ///\param [in]    m_skipPasswordCheck     If true the password will never be checked
      //-------------------------------------
      CBasicAuthentication(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager, bool skipPasswordCheck);
      
      //-------------------------------------
      ///\brief Destructor
      //-------------------------------------
      virtual ~CBasicAuthentication();

      // IAuthentifier implementation
      virtual bool isAuthenticationActive() const;
      virtual bool authenticate(const std::string & username, const std::string & password) const;
      // [END] IAuthentifier implementation

      
   private:
      //--------------------------------------------------------------
      /// \brief  Function called when the configuration has been updated
      //--------------------------------------------------------------
      void onConfigurationUpdated(boost::shared_ptr<database::entities::CConfiguration> newConfiguration);

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
      /// \brief           The notification observer
      //--------------------------------------------------------------
      boost::shared_ptr<notification::IObserver> m_observer;

      //--------------------------------------------------------------
      /// \brief           The current configuration active state
      //--------------------------------------------------------------
      bool m_isAuthenticationActive;

      //--------------------------------------------------------------
      /// \brief           The current configuration username
      //--------------------------------------------------------------
      std::string m_currentAuthenticationUsername;

      //--------------------------------------------------------------
      /// \brief           The current configuration password
      //--------------------------------------------------------------
      std::string m_currentAuthenticationPassword;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the m_currentConfiguration
      //--------------------------------------------------------------
      mutable boost::mutex m_configurationMutex;

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
