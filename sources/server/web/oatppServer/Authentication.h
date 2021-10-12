#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include "dataAccessLayer/IConfigurationManager.h"
#include "IAuthentication.h"


namespace web
{
   namespace oatppServer
   {
      //-------------------------------------
      ///\brief Class which handle authentication
      //-------------------------------------
      class CAuthentication final : public IAuthentication
      {
      public:
         //-------------------------------------
         ///\brief Constructor
         ///\param [in]    configurationManager    Configuration manager
         ///\param [in]    skipPasswordCheck       If true the password will never be checked
         //-------------------------------------
         CAuthentication(const boost::shared_ptr<dataAccessLayer::IConfigurationManager>& configurationManager,
                         bool skipPasswordCheck);
         ~CAuthentication() override;

         // IAuthentication implementation
         void authenticate(const std::shared_ptr<oatpp::web::server::HttpRequestHandler::IncomingRequest>& request) const override;
         // [END] IAuthentication implementation


      private:
         enum class Kind
         {
            kNone,
            kBasic
         };

         bool basicAuthenticate(const std::string& username,
                                const std::string& password) const;

         Kind fromConfiguration(bool skipPasswordCheck);

         void updateConfiguration();

         boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;

         // Configuration
         mutable boost::mutex m_configurationMutex;
         bool m_isAuthenticationActive;
         std::string m_currentAuthenticationUsername;
         std::string m_currentAuthenticationPassword;

         bool m_skipPasswordCheck;
         Kind m_kind;
      };
   } //namespace oatppServer
} //namespace web
