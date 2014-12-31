#include "stdafx.h"
#include "AutomationRule.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"
#include "web/rest/Result.h"

namespace web { namespace rest { namespace service {

   const std::string CAutomationRule::m_restKeyword("automation");
   const std::string CAutomationRule::m_restSubKeyword("rule");

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
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restSubKeyword), CAutomationRule::getAllRules);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)(m_restSubKeyword), CAutomationRule::createRule, CAutomationRule::transactionalMethod);

      //TODO voir la suite
      //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)(m_restSubKeyword)("*")("createRule"), CAutomationRule::createRule, CAutomationRule::transactionalMethod);
      //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)(m_restSubKeyword)("*"), CAutomationRule::updateRule, CAutomationRule::transactionalMethod);
      //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)(m_restSubKeyword)("*"), CAutomationRule::deleteRule, CAutomationRule::transactionalMethod);
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


   shared::CDataContainer CAutomationRule::getAllRules(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector<boost::shared_ptr<database::entities::CRule> > ruleList = m_rulesManager->getRules();
      shared::CDataContainer t;
      t.set(getRestKeyword(), ruleList);
      return CResult::GenerateSuccess(t);
   }

   shared::CDataContainer CAutomationRule::createRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CRule p;
         p.fillFromContent(requestContent);
         int idCreated = m_rulesManager->createRule(p);

         boost::shared_ptr<const database::entities::CRule> pluginFound = m_rulesManager->getRule(idCreated);
         return CResult::GenerateSuccess(pluginFound);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in creating a new plugin instance");
      }
   }

   shared::CDataContainer CAutomationRule::updateRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CRule instanceToUpdate;
         instanceToUpdate.fillFromContent(requestContent);

         m_rulesManager->updateRule(instanceToUpdate);

         boost::shared_ptr<const database::entities::CRule> pluginFound = m_rulesManager->getRule(instanceToUpdate.Id());
         return CResult::GenerateSuccess(pluginFound);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in updating a plugin instance");
      }
   }


   shared::CDataContainer CAutomationRule::deleteRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int instanceId = boost::lexical_cast<int>(parameters[1]);
            m_rulesManager->deleteRule(instanceId);
            return CResult::GenerateSuccess();
         }
         
         return CResult::GenerateError("invalid parameter. Can not retreive instance id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in deleting one plugin instance");
      }
   }

} //namespace service
} //namespace rest
} //namespace web 
