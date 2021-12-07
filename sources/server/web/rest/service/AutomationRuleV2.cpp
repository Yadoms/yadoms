#include "stdafx.h"
#include "AutomationRule.h"
#include "RestEndPoint.h"
#include "notification/Helpers.hpp"
#include "shared/plugin/yPluginApi/historization/PluginState.h"
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

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/interpreters", getInterpretersV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/interpreters/{interpreter}/code-template", getCodeTemplateV2));

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/rules", getRulesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "automation/rules/{id}", getRulesV2));
            //TODO

            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (RestKeyword)(RestSubKeywordRule), CAutomationRule::createRuleV1, CAutomationRule::transactionalMethod);
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (RestKeyword)(RestSubKeywordRule)("*"), CAutomationRule::deleteRuleV1, CAutomationRule::transactionalMethod);

            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("code"), CAutomationRule::getRuleCodeV1);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("log"), CAutomationRule::getRuleLogV1);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (RestKeyword)(RestSubKeywordRule)("*")("log"), CAutomationRule::deleteRuleLogV1);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("start"), CAutomationRule::startRuleV1);
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("stop"), CAutomationRule::stopRuleV1);
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (RestKeyword)(RestSubKeywordRule)("*"), CAutomationRule::updateRuleV1, CAutomationRule::transactionalMethod);
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (RestKeyword)(RestSubKeywordRule)("*")("duplicate"), CAutomationRule::duplicateRuleV1, CAutomationRule::transactionalMethod);
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (RestKeyword)(RestSubKeywordRule)("*")("code"), CAutomationRule::updateRuleCodeV1, CAutomationRule::transactionalMethod);

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

               shared::CDataContainer result;
               result.set("code-template", m_rulesManager->getRuleTemplateCode(interpreter));
               return boost::make_shared<CSuccessAnswer>(result);
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
                  auto deviceEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     deviceEntry->set("id", rule->Id());
                  if (props->empty() || props->find("name") != props->end())
                     deviceEntry->set("name", rule->Name());
                  if (props->empty() || props->find("description") != props->end())
                     deviceEntry->set("description", rule->Description());
                  if (props->empty() || props->find("interpreter") != props->end())
                     deviceEntry->set("interpreter", rule->Interpreter());
                  if (props->empty() || props->find("editor") != props->end())
                     deviceEntry->set("editor", rule->Editor());
                  if (props->empty() || props->find("model") != props->end())
                     deviceEntry->set("model", rule->Model());
                  if (props->empty() || props->find("content") != props->end())
                     deviceEntry->set("content", rule->Content());
                  if (props->empty() || props->find("autostart") != props->end())
                     deviceEntry->set("autostart", rule->AutoStart());
                  if (props->empty() || props->find("state") != props->end())
                     deviceEntry->set("state", rule->State());
                  if (props->empty() || props->find("errorMessage") != props->end())
                     deviceEntry->set("errorMessage", rule->ErrorMessage());
                  if (props->empty() || props->find("startDate") != props->end())
                     deviceEntry->set("startDate", rule->StartDate());
                  if (props->empty() || props->find("stopDate") != props->end())
                     deviceEntry->set("stopDate", rule->StopDate());

                  ruleEntries.push_back(deviceEntry);
               }

               shared::CDataContainer container;
               container.set("rules", ruleEntries);
               return boost::make_shared<CSuccessAnswer>(container);
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
      } //namespace service
   } //namespace rest
} //namespace web 
