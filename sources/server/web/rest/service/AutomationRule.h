#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "../../../automation/IRuleManager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CAutomationRule : public IRestService
         {
         public:
            CAutomationRule(boost::shared_ptr<database::IDataProvider> dataProvider,
                            boost::shared_ptr<automation::IRuleManager> rulesManager);
            virtual ~CAutomationRule();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            shared::CDataContainer getAllInterpreters(const std::vector<std::string>& parameters,
                                                      const std::string& requestContent) const;
            shared::CDataContainer getAllRules(const std::vector<std::string>& parameters,
                                               const std::string& requestContent) const;
            shared::CDataContainer getRule(const std::vector<std::string>& parameters,
                                           const std::string& requestContent) const;
            shared::CDataContainer getRuleCode(const std::vector<std::string>& parameters,
                                               const std::string& requestContent) const;
            shared::CDataContainer getRuleCodeTemplate(const std::vector<std::string>& parameters,
                                                       const std::string& requestContent) const;
            shared::CDataContainer getRuleLog(const std::vector<std::string>& parameters,
                                              const std::string& requestContent) const;
            shared::CDataContainer deleteRuleLog(const std::vector<std::string>& parameters,
                                                 const std::string& requestContent) const;
            shared::CDataContainer startRule(const std::vector<std::string>& parameters,
                                             const std::string& requestContent) const;
            shared::CDataContainer stopRule(const std::vector<std::string>& parameters,
                                            const std::string& requestContent) const;
            shared::CDataContainer createRule(const std::vector<std::string>& parameters,
                                              const std::string& requestContent) const;
            shared::CDataContainer updateRule(const std::vector<std::string>& parameters,
                                              const std::string& requestContent) const;
            shared::CDataContainer updateRuleCode(const std::vector<std::string>& parameters,
                                                  const std::string& requestContent) const;
            shared::CDataContainer deleteRule(const std::vector<std::string>& parameters,
                                              const std::string& requestContent) const;

            shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                       const std::vector<std::string>& parameters,
                                                       const std::string& requestContent) const;

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


