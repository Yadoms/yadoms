#pragma once
#include "IAuthentication.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "notification/IObserver.h"
#include "database/entities/Entities.h"


namespace authentication
{
   //-------------------------------------
   ///\brief Class which handle basic authentication (single user/password check)
   //-------------------------------------
   class CBasicAuthentication : public IAuthentication
   {
   public:
      //-------------------------------------
      ///\brief Constructor
      ///\param [in]    configurationManager    Configuration manager
      ///\param [in]    skipPasswordCheck       If true the password will never be checked
      //-------------------------------------
      CBasicAuthentication(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                           bool skipPasswordCheck);

      //-------------------------------------
      ///\brief Destructor
      //-------------------------------------
      virtual ~CBasicAuthentication();

      // IAuthentifier implementation
      bool isAuthenticationActive() const override;
      bool authenticate(const std::string& username,
                        const std::string& password) const override;
      // [END] IAuthentifier implementation


   private:
      //--------------------------------------------------------------
      /// \brief  Function which read authentication settings from database
      //--------------------------------------------------------------
      void updateConfiguration();

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
   };
} //namespace authentication
