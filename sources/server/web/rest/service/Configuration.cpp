#include "Configuration.h"
#include <shared/exception/EmptyResult.hpp>
#include "RestAccessPoint.h"
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
         std::string CConfiguration::m_restKeyword = std::string("configuration");


         CConfiguration::CConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
            : m_configurationManager(configurationManager)
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

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestAccessPoint>>> CConfiguration::accessPoints()
         {
            if (m_accessPoints != nullptr)
               return m_accessPoints;

            m_accessPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestAccessPoint>>>();
            m_accessPoints->push_back(boost::make_shared<CRestAccessPoint>(
               shared::http::ERestMethod::kGet, m_restKeyword + "/server", std::bind(&CConfiguration::getServerConfigurationV2, this,
                                                                                     std::placeholders::_1,
                                                                                     std::placeholders::_2)));
            m_accessPoints->push_back(boost::make_shared<CRestAccessPoint>(
               shared::http::ERestMethod::kGet, m_restKeyword + "/databaseVersion", std::bind(&CConfiguration::getDatabaseVersionV2, this,
                                                                                              std::placeholders::_1,
                                                                                              std::placeholders::_2)));

            {
               //boost::make_shared<CRestAccessPoint>(shared::http::ERestMethod::kGet, m_restKeyword + "/server",
               //                                     std::bind(getServerConfigurationV2, this, std::placeholders::_1, std::placeholders::_2))//,
               //TODO remettre
               //boost::make_shared<CRestAccessPoint>(shared::http::ERestMethod::kPut, m_restKeyword + "/server/reset",
               //                                     &resetServerConfiguration),
               //boost::make_shared<CRestAccessPoint>(shared::http::ERestMethod::kPut, m_restKeyword + "/server",
               //                                     &saveServerConfiguration),

               //boost::make_shared<CRestAccessPoint>(shared::http::ERestMethod::kGet, m_restKeyword + "/databaseVersion",
               //                                     &getDatabaseVersion),

               //boost::make_shared<CRestAccessPoint>(shared::http::ERestMethod::kGet, m_restKeyword + "/external/{section}",
               //                                     &getExternalConfiguration),
               //boost::make_shared<CRestAccessPoint>(shared::http::ERestMethod::kPut, m_restKeyword + "/external/{section}",
               //                                     &saveExternalConfiguration)
            }

            //TODO virer
            //m_accessPoints = boost::make_shared<std::vector<std::IRestAccessPoint>>();
            //m_accessPoints->push_back(
            //   boost::make_shared<AccessPoint>(shared::http::ERestMethod::kGet, m_restKeyword + "/server", getServerConfiguration));
            //m_accessPoints->push_back(
            //   boost::make_shared<AccessPoint>(shared::http::ERestMethod::kPut, m_restKeyword + "/server/reset", resetServerConfiguration));
            //m_accessPoints->push_back(
            //   boost::make_shared<AccessPoint>(shared::http::ERestMethod::kPut, m_restKeyword + "/server", saveServerConfiguration));

            //m_accessPoints->push_back(
            //   boost::make_shared<AccessPoint>(shared::http::ERestMethod::kGet, m_restKeyword + "/databaseVersion", getDatabaseVersion));

            //m_accessPoints->push_back(
            //   boost::make_shared<AccessPoint>(shared::http::ERestMethod::kGet, m_restKeyword + "/external", getExternalConfiguration));
            //m_accessPoints->push_back(
            //   boost::make_shared<AccessPoint>(shared::http::ERestMethod::kPut, m_restKeyword + "/external", saveExternalConfiguration));

            return m_accessPoints;
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

         boost::shared_ptr<IRestAnswer> CConfiguration::getServerConfigurationV2(const std::map<std::string,
                                                                                                std::string>& parameters,
                                                                                 const std::string& body) const
         {
            try
            {
               return boost::make_shared<CSuccessRestAnswer>(*m_configurationManager->getServerConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
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

         boost::shared_ptr<IRestAnswer> CConfiguration::getDatabaseVersionV2(const std::map<std::string,
                                                                                            std::string>& parameters,
                                                                             const std::string& body) const
         {
            try
            {
               return boost::make_shared<CSuccessRestAnswer>(m_configurationManager->getDatabaseVersion());
            }
            catch (shared::exception::CEmptyResult&)
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
                                                                 shared::CDataContainer(requestContent));

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
      } //namespace service
   } //namespace rest
} //namespace web 
