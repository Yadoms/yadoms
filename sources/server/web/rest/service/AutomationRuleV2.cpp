#include "stdafx.h"
#include "AutomationRule.h"
#include "RestEndPoint.h"
#include "notification/Helpers.hpp"
#include "shared/plugin/yPluginApi/historization/PluginState.h"
#include "web/rest/CreatedAnswer.h"
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
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CAutomationRule::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            //TODO remonter les locales de l'interpréteur (suivant la gestion par le client Angular)

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/interpreters", getInterpretersV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/interpreters/{interpreter}/icon", getInterpreterIcon));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/interpreters/{interpreter}/code-template", getCodeTemplateV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/rules", getRulesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/rules/{id}/log", getRuleLogV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/rules/{id}", getRulesV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "automation/rules", createRuleV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "automation/rules/{id}", updateRuleV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "automation/rules/{id}/log", deleteRuleLogV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "automation/rules/{id}", deleteRuleV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "automation/rules/{id}/start", startRuleV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "automation/rules/{id}/stop", stopRuleV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CAutomationRule::getInterpretersV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               const auto includeNotAvailable = request->queryParamExists("include-not-available");

               const auto interpreters = m_rulesManager->getInterpreters(includeNotAvailable);

               if (interpreters.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer interpreterEntities;
               for (const auto& interpreter : interpreters)
                  interpreterEntities.set(interpreter.first,
                                          interpreter.second ? "available" : "not-available");

               shared::CDataContainer container;
               container.set("interpreters", interpreterEntities);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getInterpreters request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getInterpreters request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get interpreters");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::getCodeTemplateV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (!request->pathVariableExists("interpreter"))
                  throw std::invalid_argument("interpreter name");
               const auto interpreter = request->pathVariable("interpreter");

               return boost::make_shared<CSuccessAnswer>(m_rulesManager->getRuleTemplateCode(interpreter),
                                                         EContentType::kPlainText);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getCodeTemplate request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getCodeTemplate request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get code template");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::getInterpreterIcon(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               if (!request->pathVariableExists("interpreter"))
                  throw std::invalid_argument("interpreter name");
               const auto interpreter = request->pathVariable("interpreter");

               const auto interpreterPath = m_rulesManager->getInterpreterPath(interpreter);

               return boost::make_shared<CSuccessAnswer>(interpreterPath / "icon.png",
                                                         EContentType::kImagePng);
            }

            catch (const std::out_of_range& exception)
            {
               YADOMS_LOG(error) << "Error processing getInterpreterIcon request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                       "Interpreter not found");
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getInterpreterIcon request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get interpreter icon");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::getRulesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto ruleId = request->pathVariableExists("id")
                                      ? boost::make_optional(static_cast<int>(std::stol(request->pathVariable("id"))))
                                      : boost::optional<int>();

               // Filtering
               const auto fromName = request->queryParamExists("from-name")
                                        ? boost::make_optional(request->queryParam("from-name"))
                                        : boost::optional<std::string>();
               const auto fromInterpreter = request->queryParamExists("from-interpreter")
                                               ? request->queryParamAsList("from-interpreter")
                                               : std::make_unique<std::set<std::string>>();
               const auto fromEditor = request->queryParamExists("from-editor")
                                          ? boost::make_optional(request->queryParam("from-editor"))
                                          : boost::optional<std::string>();
               const auto fromAutostart = request->queryParamExists("from-autostart");
               const auto fromState = request->queryParamExists("from-state")
                                         ? CHelpers::convertToEnumSet<database::entities::ERuleState>(
                                            request->queryParamAsList("from-state"))
                                         : std::make_unique<std::set<database::entities::ERuleState>>();

               // Process the request
               const auto rules = m_rulesManager->getRules(ruleId,
                                                           fromName,
                                                           *fromInterpreter,
                                                           fromEditor,
                                                           fromAutostart,
                                                           *fromState);

               if (rules.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> ruleEntries;
               for (const auto& rule : rules)
               {
                  auto ruleEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     ruleEntry->set("id", rule->Id());
                  if (props->empty() || props->find("name") != props->end())
                     ruleEntry->set("name", rule->Name());
                  if (props->empty() || props->find("description") != props->end())
                     ruleEntry->set("description", rule->Description());
                  if (props->empty() || props->find("interpreter") != props->end())
                     ruleEntry->set("interpreter", rule->Interpreter());
                  if (props->empty() || props->find("editor") != props->end())
                     ruleEntry->set("editor", rule->Editor());
                  if (props->empty() || props->find("model") != props->end())
                     ruleEntry->set("model", rule->Model());
                  if (props->empty() || props->find("content") != props->end())
                     ruleEntry->set("content", rule->Content());
                  if (props->empty() || props->find("configuration") != props->end())
                     ruleEntry->set("configuration", rule->Configuration());
                  if (props->empty() || props->find("state") != props->end())
                     ruleEntry->set("autostart", rule->AutoStart());
                  if (props->empty() || props->find("state") != props->end())
                     ruleEntry->set("state", rule->State());
                  if (props->empty() || props->find("errorMessage") != props->end())
                     ruleEntry->set("errorMessage", rule->ErrorMessage());
                  if (props->empty() || props->find("startDate") != props->end())
                     ruleEntry->set("startDate", rule->StartDate());
                  if (props->empty() || props->find("stopDate") != props->end())
                     ruleEntry->set("stopDate", rule->StopDate());
                  if (props->empty() || props->find("code") != props->end())
                  {
                     try
                     {
                        ruleEntry->set("code", m_rulesManager->getRuleCode(rule->Id()));
                     }
                     catch (const std::runtime_error&)
                     {
                        // Unable to get code (interpreter not running ?)
                        // Ignore rule entry
                        continue;
                     }
                  }

                  ruleEntries.push_back(ruleEntry);
               }

               return CHelpers::formatGetMultiItemsAnswer(ruleId.has_value(),
                                                          ruleEntries,
                                                          "rules");
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getRules request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getRules request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get rules");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::getRuleLogV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               if (!request->pathVariableExists("id"))
                  throw std::invalid_argument("rule id");
               const auto ruleId = static_cast<int>(std::stol(request->pathVariable("id")));

               const auto log = m_rulesManager->getRuleLog(ruleId);

               if (log.empty())
                  return boost::make_shared<CNoContentAnswer>();

               return boost::make_shared<CSuccessAnswer>(log,
                                                         EContentType::kPlainText);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getRuleLog request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getRuleLog request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get rule log");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::createRuleV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               shared::CDataContainer content(request->body());

               // Remove server-controlled (read-only) fields
               content.remove("id");
               content.remove("state");
               content.remove("errorMessage");
               content.remove("startDate");
               content.remove("stopDate");

               database::entities::CRule rule;
               rule.fillFromContent(content);

               if (!rule.Name.isDefined()
                  || !rule.Interpreter.isDefined()
                  || !rule.Editor.isDefined()
                  || !content.exists("code"))
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "invalid rule creation request. Need at least name, interpreter, editor and code");

               const auto idCreated = m_rulesManager->createRule(rule,
                                                                 content.get<std::string>("code"));

               return boost::make_shared<CCreatedAnswer>("automation/rules/" + std::to_string(idCreated));
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create rule");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::updateRuleV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "rule id was not provided");
               const auto ruleId = static_cast<int>(std::stol(id));

               if (request->body().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               shared::CDataContainer content(request->body());

               if (content.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               // Remove server-controlled (read-only) fields
               content.remove("id");
               content.remove("state");
               content.remove("errorMessage");
               content.remove("startDate");
               content.remove("stopDate");

               // Remove non-update-able (set-able at creation only) fields
               content.remove("interpreter");
               content.remove("editor");
               content.remove("model");

               database::entities::CRule ruleToUpdate;
               ruleToUpdate.fillFromContent(content);
               ruleToUpdate.Id = ruleId;

               m_rulesManager->updateRule(ruleToUpdate);

               if (content.exists("code"))
                  m_rulesManager->updateRuleCode(ruleId,
                                                 content.get<std::string>("code"));

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update device");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::deleteRuleV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "rule id was not provided");
                     const auto ruleId = static_cast<int>(std::stol(id));

                     m_rulesManager->deleteRule(ruleId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete device");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::deleteRuleLogV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "rule id was not provided");
                     const auto ruleId = static_cast<int>(std::stol(id));

                     m_rulesManager->deleteRuleLog(ruleId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete device");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::startRuleV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "rule id was not provided");
               const auto ruleId = static_cast<int>(std::stol(id));

               m_rulesManager->startRule(ruleId);

               return boost::make_shared<CNoContentAnswer>();
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Failed to start rule");
            }
         }

         boost::shared_ptr<IAnswer> CAutomationRule::stopRuleV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "rule id was not provided");
               const auto ruleId = static_cast<int>(std::stol(id));

               m_rulesManager->stopRule(ruleId);

               return boost::make_shared<CNoContentAnswer>();
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Failed to stop rule");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
