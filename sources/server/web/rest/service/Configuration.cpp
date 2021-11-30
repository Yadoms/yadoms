#include "stdafx.h"
#include "Configuration.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
#include "RestEndPoint.h"
#include "web/poco/RestDispatcherHelpers.hpp"
#include "web/poco/RestResult.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CConfiguration::m_restKeyword = std::string("configurations");


         CConfiguration::CConfiguration(boost::shared_ptr<database::IDataProvider> dataProvider,
                                        boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
            : m_dataProvider(std::move(dataProvider)),
              m_configurationManager(std::move(configurationManager))
         {
         }

         void CConfiguration::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("server"), CConfiguration::getServerConfigurationV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server")("reset"), CConfiguration::resetServerConfigurationV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server"), CConfiguration::saveServerConfigurationV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("databaseVersion"), CConfiguration::getDatabaseVersionV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("external")("*"), CConfiguration::getExternalConfigurationV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("external")("*"), CConfiguration::saveExternalConfigurationV1)
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::resetServerConfigurationV1(
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getServerConfigurationV1(
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::saveServerConfigurationV1(
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getDatabaseVersionV1(const std::vector<std::string>& parameters,
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::getExternalConfigurationV1(
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CConfiguration::saveExternalConfigurationV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            if (parameters.size() < 3)
               return poco::CRestResult::GenerateError("PUT /rest/configuration request : missing section name");
            const auto section = parameters[2];

            try
            {
               m_configurationManager->saveExternalConfiguration(section,
                                                                 shared::CDataContainer(requestContent));

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


         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CConfiguration::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/server", getServerConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, m_restKeyword + "/server", updateServerConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, m_restKeyword + "/server", resetServerConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/externals/{section}", getExternalConfigurationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPut, m_restKeyword + "/externals/{section}", saveExternalConfigurationV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CConfiguration::resetServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               m_configurationManager->resetServerConfiguration();
               return boost::make_shared<CSuccessAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to reset server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::getServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return boost::make_shared<CSuccessAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to get server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::updateServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (request->contentType() != EContentType::kJson)
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kUnsupportedMediaType);

               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     const auto currentConfiguration = m_configurationManager->getServerConfiguration();
                     currentConfiguration->mergeFrom(shared::CDataContainer(req->body()));

                     m_configurationManager->saveServerConfiguration(*currentConfiguration);
                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to save server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::getExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // Section
               const auto section = request->pathVariable("section", std::string());
               if (section.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "External configuration section was not provided");

               return boost::make_shared<CSuccessAnswer>(*m_configurationManager->getExternalConfiguration(section));
            }
            catch (const shared::exception::CEmptyResult&)
            {
               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "No external configuration for " + request->queryParam("section") + " section");
            }
         }

         boost::shared_ptr<IAnswer> CConfiguration::saveExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // Section
               const auto section = request->pathVariable("section", std::string());
               if (section.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "External configuration section was not provided");

               if (request->contentType() != EContentType::kJson)
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kUnsupportedMediaType);

               m_configurationManager->saveExternalConfiguration(section,
                                                                 shared::CDataContainer(request->body()));

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to save external configuration for " + request->queryParam("section") + " section");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
