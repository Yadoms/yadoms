#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "../../../automation/IRuleManager.h"

namespace web { namespace rest { namespace service {

   class CAutomationRule : public IRestService
   {
   public:
      CAutomationRule(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<automation::IRuleManager> rulesManager);
      virtual ~CAutomationRule();

      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();
      shared::CDataContainer getAllInterpreters(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  
      shared::CDataContainer getAllRules(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  
      shared::CDataContainer getRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer getRuleCode(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer createRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer updateRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer updateRuleCode(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer deleteRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  
      shared::CDataContainer restartRule(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

      shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent);

      static std::string uriEncode(const std::string& str);
      static std::string uriDecode(const std::string& str);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      boost::shared_ptr<automation::IRuleManager> m_rulesManager;
      static const std::string m_restKeyword;
      static const std::string m_restSubKeywordInterpreter;
      static const std::string m_restSubKeywordRule;
   };


} //namespace service
} //namespace rest
} //namespace web 
