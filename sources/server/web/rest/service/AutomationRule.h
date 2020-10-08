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
            virtual ~CAutomationRule() = default;

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllInterpreters(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRules(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRule(const std::vector<std::string>& parameters,
                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleCode(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleCodeTemplate(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleLog(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteRuleLog(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startRule(const std::vector<std::string>& parameters,
                                                                                  const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> stopRule(const std::vector<std::string>& parameters,
                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> createRule(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateRule(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateRuleCode(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteRule(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> duplicateRule(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
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


