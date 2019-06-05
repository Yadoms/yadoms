#include "stdafx.h"
#include "AutomationRule.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "automation/RuleException.hpp"

namespace web
{
   namespace rest
   {
      namespace service
      {
         const std::string CAutomationRule::m_restKeyword("automation");
         const std::string CAutomationRule::m_restSubKeywordInterpreter("interpreter");
         const std::string CAutomationRule::m_restSubKeywordRule("rule");

         CAutomationRule::CAutomationRule(boost::shared_ptr<database::IDataProvider> dataProvider,
                                          boost::shared_ptr<automation::IRuleManager> rulesManager)
            : m_dataProvider(dataProvider),
              m_rulesManager(rulesManager)
         {
         }


         CAutomationRule::~CAutomationRule()
         {
         }

         const std::string& CAutomationRule::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CAutomationRule::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordInterpreter), CAutomationRule::getAllInterpreters);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("codeTemplate"), CAutomationRule::getRuleCodeTemplate);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule), CAutomationRule::getAllRules);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)(m_restSubKeywordRule), CAutomationRule::createRule, CAutomationRule::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)(m_restSubKeywordRule)("*"), CAutomationRule::deleteRule, CAutomationRule::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*"), CAutomationRule::getRule);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("code"), CAutomationRule::getRuleCode);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("log"), CAutomationRule::getRuleLog);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)(m_restSubKeywordRule)("*")("log"), CAutomationRule::deleteRuleLog);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("start"), CAutomationRule::startRule);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("stop"), CAutomationRule::stopRule);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeywordRule)("*"), CAutomationRule::updateRule, CAutomationRule::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeywordRule)("*")("copy"), CAutomationRule::duplicateRule, CAutomationRule::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeywordRule)("*")("code"), CAutomationRule::updateRuleCode, CAutomationRule::transactionalMethod);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
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
               result = CResult::GenerateError(ex);
            }
            catch (...)
            {
               result = CResult::GenerateError("unknown exception automation rule rest method");
            }

            if (pTransactionalEngine)
            {
               if (CResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getAllInterpreters(const std::vector<std::string>& parameters,
                                                                                                         const std::string& requestContent) const
         {
            shared::CDataContainer t;
            t.set("interpreters", m_rulesManager->getLoadedInterpreters());
            return CResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getAllRules(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            auto rules = m_rulesManager->getRules();

            shared::CDataContainer t;
            t.set("rules", rules);
            return CResult::GenerateSuccess(t);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRule(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 3)
                  throw CRuleException("invalid parameter in URL");

               return CResult::GenerateSuccess(m_rulesManager->getRule(boost::lexical_cast<int>(parameters[2])));
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to retrieve rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleCode(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer result;
               result.set("code", m_rulesManager->getRuleCode(boost::lexical_cast<int>(parameters[2])));
               return CResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleCodeTemplate(const std::vector<std::string>& parameters,
                                                                                                          const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer result;
               result.set("code", m_rulesManager->getRuleTemplateCode(parameters[2]));
               return CResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::getRuleLog(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer result;
               result.set("log", m_rulesManager->getRuleLog(boost::lexical_cast<int>(parameters[2])));
               return CResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::deleteRuleLog(const std::vector<std::string>& parameters,
                                                                                                    const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               m_rulesManager->deleteRuleLog(boost::lexical_cast<int>(parameters[2]));
               return CResult::GenerateSuccess();
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to retrieve rule code : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::startRule(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               auto ruleId = boost::lexical_cast<int>(parameters[2]);

               //start the rule
               m_rulesManager->startRule(ruleId);

               boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(ruleId);
               return CResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to start rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in starting the rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::stopRule(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               auto ruleId = boost::lexical_cast<int>(parameters[2]);

               //stop the rule
               m_rulesManager->stopRuleAndWaitForStopped(ruleId);

               auto ruleFound = m_rulesManager->getRule(ruleId);
               return CResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to stop rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in stopping the rule");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::createRule(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               auto ruleData(boost::make_shared<database::entities::CRule>());
               shared::CDataContainer content(requestContent);
               ruleData->fillFromContent(content);

               auto idCreated = m_rulesManager->createRule(ruleData, content.get<std::string>("code"));

               boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(idCreated);
               return CResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to create rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in creating a new rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::updateRule(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 3)
                  throw CRuleException("invalid parameter in URL");

               auto ruleId = boost::lexical_cast<int>(parameters[2]);
               auto ruleData(boost::make_shared<database::entities::CRule>());
               ruleData->fillFromSerializedString(requestContent);

               // Check for supported modifications
               if (ruleData->Id.isDefined() && ruleData->Id != ruleId)
                  throw CRuleException("Rule Id is not modifiable");

               ruleData->Id = ruleId;
               m_rulesManager->updateRule(ruleData);

               boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(ruleId);
               return CResult::GenerateSuccess(ruleFound);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to update rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating a rule");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::updateRuleCode(const std::vector<std::string>& parameters,
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
               return CResult::GenerateSuccess(result);
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to update rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating a rule");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::duplicateRule(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 4)
                  throw CRuleException("invalid parameter in URL");

               shared::CDataContainer content(requestContent);
               if(!content.containsValue("name"))
                  throw CRuleException("No name provided for the rule copy");

               auto name = content.get<std::string>("name");
               if (name.empty())
                  throw CRuleException("empty name provided for the rule copy");
               auto ruleId = boost::lexical_cast<int>(parameters[2]);

               auto newRuleId = m_rulesManager->duplicateRule(ruleId, name);

               const boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(newRuleId);
               return CResult::GenerateSuccess(ruleFound);
            }

            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to duplicate rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in duplicating a rule");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CAutomationRule::deleteRule(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() != 3)
                  throw CRuleException("invalid parameter in URL");

               auto instanceId = boost::lexical_cast<int>(parameters[2]);
               m_rulesManager->deleteRule(instanceId);
               return CResult::GenerateSuccess();
            }
            catch (CRuleException& e)
            {
               return CResult::GenerateError(std::string("Fail to delete rule : ") + e.what());
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting a rule");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 


