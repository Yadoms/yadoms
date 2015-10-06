#include "stdafx.h"
#include "AutomationRule.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/Result.h"
#include "automation/RuleException.hpp"
#include <Poco/URI.h>

namespace web { namespace rest { namespace service {

   const std::string CAutomationRule::m_restKeyword("automation");
   const std::string CAutomationRule::m_restSubKeywordInterpreter("interpreter");
   const std::string CAutomationRule::m_restSubKeywordRule("rule");

   CAutomationRule::CAutomationRule(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<automation::IRuleManager> rulesManager)
      :m_dataProvider(dataProvider), m_rulesManager(rulesManager)
   {
   }


   CAutomationRule::~CAutomationRule()
   {
   }

   const std::string & CAutomationRule::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CAutomationRule::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordInterpreter), CAutomationRule::getAllInterpreters);
      
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule), CAutomationRule::getAllRules);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)(m_restSubKeywordRule), CAutomationRule::createRule, CAutomationRule::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)(m_restSubKeywordRule)("*"), CAutomationRule::deleteRule, CAutomationRule::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*"), CAutomationRule::getRule);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("code"), CAutomationRule::getRuleCode);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeywordRule)("*")("log"), CAutomationRule::getRuleLog);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeywordRule)("*"), CAutomationRule::updateRule, CAutomationRule::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeywordRule)("*")("code"), CAutomationRule::updateRuleCode, CAutomationRule::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeywordRule)("*")("restart"), CAutomationRule::restartRule, CAutomationRule::transactionalMethod);
   }

   shared::CDataContainer CAutomationRule::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      shared::CDataContainer result;
      try
      {
         if(pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch(std::exception &ex)
      {
         result = CResult::GenerateError(ex);
      }
      catch(...)
      {
         result = CResult::GenerateError("unknown exception automation rule rest method");
      }

      if(pTransactionalEngine)
      {
         if(CResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }

   std::string CAutomationRule::uriEncode(const std::string& str)
   {
      std::string out;
      Poco::URI::encode(str, std::string(), out);
      return out;
   }

   std::string CAutomationRule::uriDecode(const std::string& str)
   {
      std::string out;
      Poco::URI::decode(str, out);
      return out;
   }

   shared::CDataContainer CAutomationRule::getAllInterpreters(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      shared::CDataContainer t;
      t.set("interpreters", m_rulesManager->getAvailableInterpreters());
      return CResult::GenerateSuccess(t);
   }

   shared::CDataContainer CAutomationRule::getAllRules(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector<boost::shared_ptr<database::entities::CRule> > ruleList = m_rulesManager->getRules();
      shared::CDataContainer t;
      t.set("rules", ruleList);
      return CResult::GenerateSuccess(t);
   }

   shared::CDataContainer CAutomationRule::getRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 3)
            throw CRuleException("invalid parameter in URL");

         return CResult::GenerateSuccess(m_rulesManager->getRule(boost::lexical_cast<int>(parameters[2])));
      }
      catch (CRuleException& e)
      {
         return CResult::GenerateError(std::string("Fail to retreive rule : ") + e.what());
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving the rule");
      }
   }

   shared::CDataContainer CAutomationRule::getRuleCode(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 4)
            throw CRuleException("invalid parameter in URL");

         shared::CDataContainer result;
         result.set("code", uriEncode(m_rulesManager->getRuleCode(boost::lexical_cast<int>(parameters[2]))));
         return CResult::GenerateSuccess(result);
      }
      catch (CRuleException& e)
      {
         return CResult::GenerateError(std::string("Fail to retreive rule code : ") + e.what());
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving the rule");
      }
   }

   shared::CDataContainer CAutomationRule::getRuleLog(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 4)
            throw CRuleException("invalid parameter in URL");

         shared::CDataContainer result;
         result.set("log", uriEncode(m_rulesManager->getRuleLog(boost::lexical_cast<int>(parameters[2]))));
         return CResult::GenerateSuccess(result);
      }
      catch (CRuleException& e)
      {
         return CResult::GenerateError(std::string("Fail to retreive rule code : ") + e.what());
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving the rule");
      }
   }


   shared::CDataContainer CAutomationRule::createRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
         ruleData->fillFromContent(requestContent);

         int idCreated = m_rulesManager->createRule(ruleData, uriDecode(requestContent.get<std::string>("code")));

         boost::shared_ptr<const database::entities::CRule> ruleFound = m_rulesManager->getRule(idCreated);
         return CResult::GenerateSuccess(ruleFound);
      }
      catch (CRuleException& e)
      {
         return CResult::GenerateError(std::string("Fail to create rule : ") + e.what());
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in creating a new rule");
      }
   }

   shared::CDataContainer CAutomationRule::updateRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 3)
            throw CRuleException("invalid parameter in URL");

         int ruleId = boost::lexical_cast<int>(parameters[2]);
         boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
         ruleData->fillFromContent(requestContent);

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
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in updating a rule");
      }
   }

   shared::CDataContainer CAutomationRule::updateRuleCode(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 4)
            throw CRuleException("invalid parameter in URL");

         int ruleId = boost::lexical_cast<int>(parameters[2]);
         boost::shared_ptr<database::entities::CRule> ruleData(new database::entities::CRule);
         ruleData->fillFromContent(requestContent);

         // Check for supported modifications
         if (ruleData->Id.isDefined())
            throw CRuleException("Rule Id is not modifiable");

         std::string code = uriDecode(requestContent.get<std::string>("code"));
         if (code.empty())
            throw CRuleException("No code provided for the rule");

         m_rulesManager->updateRuleCode(ruleId, code);

         shared::CDataContainer result;
         result.set("code", uriEncode(m_rulesManager->getRuleCode(ruleId)));
         return CResult::GenerateSuccess(result);
      }
      catch (CRuleException& e)
      {
         return CResult::GenerateError(std::string("Fail to update rule : ") + e.what());
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in updating a rule");
      }
   }


   shared::CDataContainer CAutomationRule::deleteRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 3)
            throw CRuleException("invalid parameter in URL");

         int instanceId = boost::lexical_cast<int>(parameters[2]);
         m_rulesManager->deleteRule(instanceId);
         return CResult::GenerateSuccess();
      }
      catch (CRuleException& e)
      {
         return CResult::GenerateError(std::string("Fail to delete rule : ") + e.what());
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in deleting a rule");
      }
   }


   shared::CDataContainer CAutomationRule::restartRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size() != 4)
            throw CRuleException("invalid parameter in URL");

         int instanceId = boost::lexical_cast<int>(parameters[2]);
         m_rulesManager->restartRule(instanceId);
         return CResult::GenerateSuccess();
      }
      catch (CRuleException& e)
      {
         std::string err = std::string("Fail to restart rule : ") + e.what();
         YADOMS_LOG(error) << err;
         return CResult::GenerateError(err);
      }
      catch(std::exception &ex)
      {
         YADOMS_LOG(error) << ex.what();
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         std::string err ("unknown exception in restarting a rule");
         YADOMS_LOG(error) << err;
         return CResult::GenerateError(err);
      }
   }

} //namespace service
} //namespace rest
} //namespace web 
