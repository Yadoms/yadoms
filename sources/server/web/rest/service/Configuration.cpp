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
            //TODO faire le ménage sur ce qui est utile
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("system"), CConfiguration::getSystemConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("system")("reset"), CConfiguration::resetSystemConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("system"), CConfiguration::saveSystemConfiguration);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CConfiguration::getAllConfigurations);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CConfiguration::getSectionConfigurations);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*"), CConfiguration::getConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")("*"), CConfiguration::updateOneConfiguration);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword), CConfiguration::updateAllConfigurations);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("*")("*"), CConfiguration::deleteOneConfiguration);
         }

         shared::CDataContainer CConfiguration::resetSystemConfiguration(const std::vector<std::string>& parameters,
                                                                         const std::string& requestContent) const
         {
            try
            {
               m_configurationManager->resetSystemConfiguration();
               return CResult::GenerateSuccess(m_configurationManager->getSystemConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError("Fail to reset system configuration");
            }
         }

         shared::CDataContainer CConfiguration::getSystemConfiguration(const std::vector<std::string>& parameters,
                                                                       const std::string& requestContent) const
         {
            auto keyname = std::string();
            if (parameters.size() > 2)
               keyname = parameters[2];

            try
            {
               if (!keyname.empty())
                  return CResult::GenerateSuccess(m_configurationManager->getSystemConfiguration(keyname));
               return CResult::GenerateSuccess(m_configurationManager->getSystemConfiguration());
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError((boost::format("[Section = system ; Name = %1%] not found.") % keyname).str());
            }
         }

         shared::CDataContainer CConfiguration::saveSystemConfiguration(const std::vector<std::string>& parameters,
                                                                        const std::string& requestContent) const
         {
            try
            {
               m_configurationManager->saveSystemConfiguration(shared::CDataContainer(requestContent));
               return CResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating all configuration");
            }
         }


         shared::CDataContainer CConfiguration::getConfiguration(const std::vector<std::string>& parameters,
                                                                 const std::string& requestContent) const
         {
            std::string section = "";
            std::string keyname = "";
            if (parameters.size() > 1)
               section = parameters[1];
            if (parameters.size() > 2)
               keyname = parameters[2];

            try
            {
               const auto configFound = m_configurationManager->getConfiguration(section, keyname);
               return CResult::GenerateSuccess(configFound);
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError((boost::format("[Section = %1% ; Name = %2%] not found.") % section % keyname).str());
            }
         }

         shared::CDataContainer CConfiguration::getSectionConfigurations(const std::vector<std::string>& parameters,
                                                                         const std::string& requestContent) const
         {
            std::string section = "";
            if (parameters.size() > 1)
               section = parameters[1];


            const auto config = m_configurationManager->getConfigurations(section);
            shared::CDataContainer collection;
            collection.set(m_restKeyword, config);
            return CResult::GenerateSuccess(collection);
         }

         shared::CDataContainer CConfiguration::getAllConfigurations(const std::vector<std::string>& parameters,
                                                                     const std::string& requestContent) const
         {
            const auto config = m_configurationManager->getConfigurations();
            shared::CDataContainer collection;
            collection.set(m_restKeyword, config);
            return CResult::GenerateSuccess(collection);
         }

         shared::CDataContainer CConfiguration::updateOneConfiguration(const std::vector<std::string>& parameters,
                                                                       const std::string& requestContent) const
         {
            try
            {
               database::entities::CConfiguration configToUpdate;
               configToUpdate.fillFromSerializedString(requestContent);

               std::string section = "";
               std::string keyname = "";
               if (parameters.size() > 1)
                  section = parameters[1];
               if (parameters.size() > 2)
                  keyname = parameters[2];


               if ((configToUpdate.Section().empty() && configToUpdate.Name().empty()) ||
                  (boost::iequals(configToUpdate.Section(), section) && boost::iequals(configToUpdate.Name(), keyname)))
               {
                  //ensure section and name are corretly filled
                  configToUpdate.Section = section;
                  configToUpdate.Name = keyname;
                  //commit changes to database
                  m_configurationManager->updateConfiguration(configToUpdate);

                  return CResult::GenerateSuccess(m_configurationManager->getConfiguration(section, keyname));
               }
               else
               {
                  return CResult::GenerateError("section and name in query content does not match to rest url");
               }
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

         shared::CDataContainer CConfiguration::updateAllConfigurations(const std::vector<std::string>& parameters,
                                                                        const std::string& requestContent) const
         {
            try
            {
               auto listToUpdate = shared::CDataContainer(requestContent).get<std::vector<boost::shared_ptr<database::entities::CConfiguration>>>(
                  m_restKeyword);

               for (auto i = listToUpdate.begin(); i != listToUpdate.end(); ++i)
               {
                  m_configurationManager->updateConfiguration(*i->get());
               }

               return getAllConfigurations(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating all configuration");
            }
         }


         shared::CDataContainer CConfiguration::deleteOneConfiguration(const std::vector<std::string>& parameters,
                                                                       const std::string& requestContent) const
         {
            std::string section = "";
            std::string keyname = "";
            if (parameters.size() > 1)
               section = parameters[1];
            if (parameters.size() > 2)
               keyname = parameters[2];

            if (!section.empty() && !keyname.empty())
            {
               database::entities::CConfiguration configToRemove;
               configToRemove.Section = section;
               configToRemove.Name = keyname;
               m_configurationManager->removeConfiguration(configToRemove);
            }

            return CResult::GenerateSuccess();
         }
      } //namespace service
   } //namespace rest
} //namespace web 
