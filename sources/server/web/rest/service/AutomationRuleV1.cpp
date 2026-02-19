#include <utility>

#include "stdafx.h"
#include "AutomationRule.h"
#include "web/poco/RestResult.h"
#include "automation/RuleException.hpp"
#include "web/poco/RestDispatcherHelpers.hpp"

namespace web
{
   namespace rest
   {
      namespace service
      {
         const std::string CAutomationRule::RestKeyword("automation");
         const std::string CAutomationRule::RestSubKeywordInterpreter("interpreter");
         const std::string CAutomationRule::RestSubKeywordRule("rule");

         CAutomationRule::CAutomationRule(boost::shared_ptr<database::IDataProvider> dataProvider,
                                          boost::shared_ptr<automation::IRuleManager> rulesManager)
            : m_dataProvider(std::move(dataProvider)),
              m_rulesManager(std::move(rulesManager))
         {
         }

         const std::string& CAutomationRule::getRestKeyword()
         {
            return RestKeyword;
         }

         void CAutomationRule::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordInterpreter), CAutomationRule::getAllInterpretersV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordInterpreter)("available"),
                                        CAutomationRule::getAvailableInterpretersV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("codeTemplate"),
                                        CAutomationRule::getRuleCodeTemplateV1)

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule), CAutomationRule::getAllRulesV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (RestKeyword)(RestSubKeywordRule), CAutomationRule::createRuleV1,
                                                        CAutomationRule::transactionalMethod)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (RestKeyword)(RestSubKeywordRule)("*"), CAutomationRule::deleteRuleV1,
                                                        CAutomationRule::transactionalMethod)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*"), CAutomationRule::getRuleV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("code"), CAutomationRule::getRuleCodeV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("log"), CAutomationRule::getRuleLogV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (RestKeyword)(RestSubKeywordRule)("*")("log"), CAutomationRule::deleteRuleLogV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("start"), CAutomationRule::startRuleV1)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (RestKeyword)(RestSubKeywordRule)("*")("stop"), CAutomationRule::stopRuleV1)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (RestKeyword)(RestSubKeywordRule)("*"), CAutomationRule::updateRuleV1,
                                                        CAutomationRule::transactionalMethod)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (RestKeyword)(RestSubKeywordRule)("*")("duplicate"),
                                                        CAutomationRule::duplicateRuleV1, CAutomationRule::transactionalMethod)
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (RestKeyword)(RestSubKeywordRule)("*")("code"),
                                                        CAutomationRule::updateRuleCodeV1, CAutomationRule::transactionalMethod)
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::transactionalMethod(
            poco::CRestDispatcher::CRestMethodHandler realMethod,
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters,
                                   requestContent);
            }
            catch (std::exception& ex)
            {
               result = poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               result = poco::CRestResult::GenerateError("unknown exception automation rule rest method");
            }

            if (pTransactionalEngine)
            {
               if (poco::CRestResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getAllInterpretersV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            shared::CDataContainer t;
            t.set("interpreters", m_rulesManager->getLoadedInterpreters());
            return poco::CRestResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getAvailableInterpretersV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            shared::CDataContainer t;
            t.set("interpreters", m_rulesManager->getAvailableInterpreters());
            return poco::CRestResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getAllRulesV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            const auto rules = m_rulesManager->getRules();

            shared::CDataContainer t;
            t.set("rules", rules);
            return poco::CRestResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleV1(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 3)
                  throw CRuleException("invalid parameter in URL");

               return poco::CRestResult::GenerateSuccess(m_rulesManager->getRule(boost::lexical_cast<int>(parameters[2])));
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to retrieve rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleCodeV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer result;
               result.set("code", m_rulesManager->getRuleCode(boost::lexical_cast<int>(parameters[2])));
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleCodeTemplateV1(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer result;
               result.set("code", m_rulesManager->getRuleTemplateCode(parameters[2]));
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleLogV1(const std::vector<std::string>& parameters,
                                                                                                   const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer result;
               result.set("log", m_rulesManager->getRuleLog(boost::lexical_cast<int>(parameters[2])));
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::deleteRuleLogV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               m_rulesManager->deleteRuleLog(boost::lexical_cast<int>(parameters[2]));
               return poco::CRestResult::GenerateSuccess();
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::startRuleV1(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               const auto ruleId = boost::lexical_cast<int>(parameters[2]);

               //start the rule
               m_rulesManager->startRule(ruleId);

               const boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(ruleId);
               return poco::CRestResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to start rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in starting the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::stopRuleV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               const auto ruleId = boost::lexical_cast<int>(parameters[2]);

               //stop the rule
               m_rulesManager->stopRuleAndWaitForStopped(ruleId);

               const auto ruleFound = m_rulesManager->getRule(ruleId);
               return poco::CRestResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to stop rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in stopping the rule");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::createRuleV1(const std::vector<std::string>& parameters,
                                                                                                   const std::string& requestContent) const
         {
            try
            {
               const shared::CDataContainer content(requestContent);
               database::entities::CRule ruleData;
               ruleData.fillFromContent(content);

               const auto idCreated = m_rulesManager->createRule(ruleData, content.get<std::string>("code"));

               const boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(idCreated);
               return poco::CRestResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to create rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::updateRuleV1(const std::vector<std::string>& parameters,
                                                                                                   const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 3)
                  throw CRuleException("invalid parameter in URL");

               const auto ruleId = boost::lexical_cast<int>(parameters[2]);
               database::entities::CRule ruleData;
               ruleData.fillFromSerializedString(requestContent);

               // Check for supported modifications
               if (ruleData.Id.isDefined() && ruleData.Id != ruleId)
                  throw CRuleException("Rule Id is not modifiable");

               ruleData.Id = ruleId;
               m_rulesManager->updateRule(ruleData);

               const boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(ruleId);
               return poco::CRestResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to update rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in updating a rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::updateRuleCodeV1(const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               auto ruleId = boost::lexical_cast<int>(parameters[2]);
               auto ruleData(boost::make_shared<database::entities::CRule>());
               shared::CDataContainer content(requestContent);
               ruleData->fillFromContent(content);

               // Check for supported modifications
               if (ruleData->Id.isDefined())
                  throw CRuleException("Rule Id is not modifiable");

               auto code = content.get<std::string>("code");
               if (code.empty())
                  throw CRuleException("No code provided for the rule");

               m_rulesManager->updateRuleCode(ruleId, code);

               shared::CDataContainer result;
               result.set("code", m_rulesManager->getRuleCode(ruleId));
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to update rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in updating a rule");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::duplicateRuleV1(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               const shared::CDataContainer content(requestContent);
               if (!content.containsValue("name"))
                  throw CRuleException("No name provided for the rule copy");

               const auto name = content.get<std::string>("name");
               if (name.empty())
                  throw CRuleException("empty name provided for the rule copy");
               const auto ruleId = boost::lexical_cast<int>(parameters[2]);

               const auto newRuleId = m_rulesManager->duplicateRule(ruleId, name);

               const boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(newRuleId);
               return poco::CRestResult::GenerateSuccess(ruleFound);
            }

            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to duplicate rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in duplicating a rule");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::deleteRuleV1(const std::vector<std::string>& parameters,
                                                                                                   const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 3)
                  throw CRuleException("invalid parameter in URL");

               const auto instanceId = boost::lexical_cast<int>(parameters[2]);
               m_rulesManager->deleteRule(instanceId);
               return poco::CRestResult::GenerateSuccess();
            }
            catch (CRuleException& e)
            {
               return poco::CRestResult::GenerateError(std::string("Fail to delete rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in deleting a rule");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
