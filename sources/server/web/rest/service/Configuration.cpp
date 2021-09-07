#include "stdafx.h"
#include "Configuration.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
#include "RestEndPoint.h"
#include "web/poco/RestResult.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/SuccessAnswer.h"

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

         void CConfiguration::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("server"), CConfiguration::getServerConfiguration)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server")("reset"), CConfiguration::resetServerConfiguration)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server"), CConfiguration::saveServerConfiguration)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("databaseVersion"), CConfiguration::getDatabaseVersion)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("external")("*"), CConfiguration::getExternalConfiguration)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("external")("*"), CConfiguration::saveExternalConfiguration)
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CConfiguration::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/server", getServerConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/server", getServerConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPut, m_restKeyword + "/server", saveServerConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, m_restKeyword + "/server", resetServerConfigurationV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/databaseVersion", getDatabaseVersionV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/external", getExternalConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPut, m_restKeyword + "/external", saveExternalConfigurationV2));

            return m_endPoints;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::resetServerConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               m_configurationManager->resetServerConfiguration();
               return poco::CRestResult::GenerateSuccess(m_configurationManager->getServerConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return poco::CRestResult::GenerateError("Fail to reset server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::resetServerConfigurationV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               m_configurationManager->resetServerConfiguration();
               return boost::make_shared<CSuccessAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to reset server configuration");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getServerConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               return poco::CRestResult::GenerateSuccess(m_configurationManager->getServerConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return poco::CRestResult::GenerateError("Fail to get server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::getServerConfigurationV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               return boost::make_shared<CSuccessAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
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
               return poco::CRestResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("Fail to save server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::saveServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               m_configurationManager->saveServerConfiguration(shared::CDataContainer(request->body()));
               return boost::make_shared<CSuccessAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to get database version");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getDatabaseVersion(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               return poco::CRestResult::GenerateSuccess(m_configurationManager->getDatabaseVersion());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return poco::CRestResult::GenerateError("Fail to get server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::getDatabaseVersionV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               return boost::make_shared<CSuccessAnswer>(m_configurationManager->getDatabaseVersion());
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to get database version");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getExternalConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            if (parameters.size() < 3)
               return poco::CRestResult::GenerateError("GET /rest/configuration request : missing section name");
            const auto section = parameters[2];

            try
            {
               const auto configFound = m_configurationManager->getExternalConfiguration(section);
               return poco::CRestResult::GenerateSuccess(configFound);
            }
            catch (shared::exception::CEmptyResult&)
            {
               return poco::CRestResult::GenerateError((boost::format("[Section = %1%] not found.") % section).str());
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::getExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return boost::make_shared<CSuccessAnswer>(m_configurationManager->getExternalConfiguration(request->parameter("section")));
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to get external configuration");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::saveExternalConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            if (parameters.size() < 3)
               return poco::CRestResult::GenerateError("PUT /rest/configuration request : missing section name");
            const auto section = parameters[2];

            try
            {
               m_configurationManager->saveExternalConfiguration(section,
                                                                 requestContent);

               return poco::CRestResult::GenerateSuccess(m_configurationManager->getExternalConfiguration(section));
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("Unknown exception in updating a configuration value");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::saveExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (request->contentType() != EContentType::kPlainText)
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "Fail to save external configuration, provided configuration must be plain text contentType");

               m_configurationManager->saveExternalConfiguration(request->parameter("section"),
                                                                 request->body());

               return boost::make_shared<CSuccessAnswer>(m_configurationManager->getExternalConfiguration(request->parameter("section")));
            }
            catch (std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to save external configuration");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
