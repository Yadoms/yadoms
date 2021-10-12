#include "stdafx.h"
#include "Authentication.h"

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <Poco/MD5Engine.h>
#include <shared/Log.h>

namespace web
{
   namespace oatppServer
   {
      CAuthentication::CAuthentication(const boost::shared_ptr<dataAccessLayer::IConfigurationManager>& configurationManager,
                                       bool skipPasswordCheck)
         : m_configurationManager(configurationManager),
           m_isAuthenticationActive(false),
           m_skipPasswordCheck(skipPasswordCheck),
           m_kind(fromConfiguration(m_skipPasswordCheck))
      {
         configurationManager->subscribeOnServerConfigurationChanged(
            [&](const boost::shared_ptr<const shared::CDataContainer>&)
            {
               m_kind = fromConfiguration(m_skipPasswordCheck);
            });
      }

      CAuthentication::~CAuthentication()
      {
         try
         {
            if (m_skipPasswordCheck)
               return;
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Error destroying BasicAuthentication : " << ex.what();
         }
      }

      void CAuthentication::authenticate(const std::shared_ptr<oatpp::web::server::HttpRequestHandler::IncomingRequest>& request) const
      {
         switch (m_kind)
         {
         case Kind::kNone:
            return;

         case Kind::kBasic:
            {
               const auto authorizationHeader = request->getHeader(oatpp::web::protocol::http::Header::AUTHORIZATION);

               const auto basicAuthorizationHandler = std::make_shared<oatpp::web::server::handler::BasicAuthorizationHandler>();
               const auto authorizationObject = std::static_pointer_cast<oatpp::web::server::handler::DefaultBasicAuthorizationObject>(
                  basicAuthorizationHandler->handleAuthorization(authorizationHeader));

               if (!basicAuthenticate(authorizationObject->userId->c_str(),
                                      authorizationObject->password->c_str()))
               {
                  oatpp::web::protocol::http::Headers responseHeaders;
                  basicAuthorizationHandler->addErrorResponseHeaders(responseHeaders);
                  throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_401,
                                                              "Unauthorized",
                                                              responseHeaders);
               }
               return;
            }

         default: // NOLINT(clang-diagnostic-covered-switch-default)
            throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_500,
                                                        "Invalid configured authorization");
         }
      }

      bool CAuthentication::basicAuthenticate(const std::string& username,
                                              const std::string& password) const
      {
         boost::lock_guard<boost::mutex> lock(m_configurationMutex);

         try
         {
            Poco::MD5Engine md5;
            md5.update(password);
            const auto cypherPassword(Poco::DigestEngine::digestToHex(md5.digest()));

            return boost::iequals(username, m_currentAuthenticationUsername)
               && boost::equals(cypherPassword, m_currentAuthenticationPassword);
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Fail to read configuration value :" << ex.what();
         }

         return true;
      }

      CAuthentication::Kind CAuthentication::fromConfiguration(bool skipPasswordCheck)
      {
         if (skipPasswordCheck)
            return Kind::kNone;

         updateConfiguration();

         if (!m_isAuthenticationActive)
            return Kind::kNone;

         if (m_currentAuthenticationUsername.empty() || m_currentAuthenticationPassword.empty())
         {
            YADOMS_LOG(error) << "Basic authentication is active but username or password is empty";
            return Kind::kNone;
         }

         return Kind::kBasic;
      }

      void CAuthentication::updateConfiguration()
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
   } //namespace oatppServer
} //namespace web
