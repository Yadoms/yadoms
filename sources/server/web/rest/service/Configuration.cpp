#include "stdafx.h"
#include "Configuration.h"
#include <shared/exception/EmptyResult.hpp>
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"

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


         CConfiguration::~CConfiguration()
         {
         }


         void CConfiguration::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("server"), CConfiguration::getServerConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server")("reset"), CConfiguration::resetServerConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("server"), CConfiguration::saveServerConfiguration);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CConfiguration::getExternalConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*"), CConfiguration::saveExternalConfiguration);
         }

         shared::CDataContainer CConfiguration::resetServerConfiguration(const std::vector<std::string>& parameters,
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

         shared::CDataContainer CConfiguration::getServerConfiguration(const std::vector<std::string>& parameters,
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

         shared::CDataContainer CConfiguration::saveServerConfiguration(const std::vector<std::string>& parameters,
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


         shared::CDataContainer CConfiguration::getExternalConfiguration(const std::vector<std::string>& parameters,
                                                                         const std::string& requestContent) const
         {
            if (parameters.size() < 2)
               return CResult::GenerateError("GET /rest/configuration request : missing section name");
            const auto section = parameters[1];

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

         shared::CDataContainer CConfiguration::saveExternalConfiguration(const std::vector<std::string>& parameters,
                                                                          const std::string& requestContent) const
         {
            //TODO ajouter une protection : on ne doit pas être capable d'écraser DatabaseVersion
            if (parameters.size() < 2)
               return CResult::GenerateError("PUT /rest/configuration request : missing section name");
            const auto section = parameters[1];

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
               return CResult::GenerateError("unknown exception in updating a configuration value");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
