#include "Configuration.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
#include "RestEndPoint.h"
#include "stdafx.h"
#include "web/rest/RestDispatcher.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "web/rest/ResultV2.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CConfiguration::m_restKeyword = std::string("configurations");


         CConfiguration::CConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
            : m_configurationManager(std::move(configurationManager))
         {
         }

         void CConfiguration::configurePocoDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("server"), CConfiguration::getServerConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server")("reset"), CConfiguration::resetServerConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server"), CConfiguration::saveServerConfiguration);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("databaseVersion"), CConfiguration::getDatabaseVersion);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("external")("*"), CConfiguration::getExternalConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("external")("*"), CConfiguration::saveExternalConfiguration);
         }

#define EP(verb, path, handler) \
          boost::make_shared<CRestEndPoint>(shared::http::ERestVerb::verb, \
          path, \
          [this](boost::shared_ptr<IRequest> request) \
               { return handler(request); })


         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CConfiguration::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(EP(kGet, m_restKeyword + "/server", getServerConfigurationV2));
            m_endPoints->push_back(EP(kPut, m_restKeyword + "/server", saveServerConfigurationV2));
            m_endPoints->push_back(EP(kDelete, m_restKeyword + "/server", resetServerConfigurationV2));

            m_endPoints->push_back(EP(kGet, m_restKeyword + "/databaseVersion", getDatabaseVersionV2));

            m_endPoints->push_back(EP(kGet, m_restKeyword + "/external", getExternalConfigurationV2));
            m_endPoints->push_back(EP(kPut, m_restKeyword + "/external", saveExternalConfigurationV2));

            return m_endPoints;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::resetServerConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               m_configurationManager->resetServerConfiguration();
               return CResult::GenerateSuccess(m_configurationManager->getServerConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError("Fail to reset server configuration");
            }
         }

         boost::shared_ptr<IRestAnswer> CConfiguration::resetServerConfigurationV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               m_configurationManager->resetServerConfiguration();
               return boost::make_shared<CSuccessRestAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kNotFound,
                                                           "Fail to reset server configuration");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getServerConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               return CResult::GenerateSuccess(m_configurationManager->getServerConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError("Fail to get server configuration");
            }
         }

         boost::shared_ptr<IRestAnswer> CConfiguration::getServerConfigurationV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               return boost::make_shared<CSuccessRestAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kNotFound,
                                                           "Fail to get server configuration");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::saveServerConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               m_configurationManager->saveServerConfiguration(shared::CDataContainer(requestContent));
               return CResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("Fail to save server configuration");
            }
         }

         boost::shared_ptr<IRestAnswer> CConfiguration::saveServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               m_configurationManager->saveServerConfiguration(shared::CDataContainer(request->body()));
               return boost::make_shared<CSuccessRestAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kNotFound,
                                                           "Fail to get database version");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getDatabaseVersion(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               return CResult::GenerateSuccess(m_configurationManager->getDatabaseVersion());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError("Fail to get server configuration");
            }
         }

         boost::shared_ptr<IRestAnswer> CConfiguration::getDatabaseVersionV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               return boost::make_shared<CSuccessRestAnswer>(m_configurationManager->getDatabaseVersion());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kNotFound,
                                                           "Fail to get database version");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getExternalConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            if (parameters.size() < 3)
               return CResult::GenerateError("GET /rest/configuration request : missing section name");
            const auto section = parameters[2];

            try
            {
               const auto configFound = m_configurationManager->getExternalConfiguration(section);
               return CResult::GenerateSuccess(configFound);
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError((boost::format("[Section = %1%] not found.") % section).str());
            }
         }

         boost::shared_ptr<IRestAnswer> CConfiguration::getExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return boost::make_shared<CSuccessRestAnswer>(m_configurationManager->getExternalConfiguration(request->parameter("section")));
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kNotFound,
                                                           "Fail to get external configuration");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::saveExternalConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            if (parameters.size() < 3)
               return CResult::GenerateError("PUT /rest/configuration request : missing section name");
            const auto section = parameters[2];

            try
            {
               m_configurationManager->saveExternalConfiguration(section,
                                                                 requestContent);

               return CResult::GenerateSuccess(m_configurationManager->getExternalConfiguration(section));
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("Unknown exception in updating a configuration value");
            }
         }

         boost::shared_ptr<IRestAnswer> CConfiguration::saveExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (request->contentType() != EContentType::kPlainText)
                  return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kBadRequest,
                                                              "Fail to save external configuration, provided configuration must be plain text contentType");

               m_configurationManager->saveExternalConfiguration(request->parameter("section"),
                                                                 request->body());

               return boost::make_shared<CSuccessRestAnswer>(m_configurationManager->getExternalConfiguration(request->parameter("section")));
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorRestAnswer>(shared::http::ECodes::kInternalServerError,
                                                           "Fail to save external configuration");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
