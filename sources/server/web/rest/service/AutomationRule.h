#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "../../../automation/IRuleManager.h"
#include "web/poco/RestDispatcher.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CAutomationRule final : public IRestService
         {
         public:
            CAutomationRule(boost::shared_ptr<database::IDataProvider> dataProvider,
                            boost::shared_ptr<automation::IRuleManager> rulesManager);
            ~CAutomationRule() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            static const std::string& getRestKeyword();
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllInterpretersV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAvailableInterpretersV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRulesV1(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleV1(const std::vector<std::string>& parameters,
                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleCodeV1(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleCodeTemplateV1(const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getRuleLogV1(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteRuleLogV1(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startRuleV1(const std::vector<std::string>& parameters,
                                                                                  const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> stopRuleV1(const std::vector<std::string>& parameters,
                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> createRuleV1(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateRuleV1(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateRuleCodeV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteRuleV1(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> duplicateRuleV1(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(poco::CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            boost::shared_ptr<IAnswer> getInterpretersV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getRulesV2(const boost::shared_ptr<IRequest>& request) const;


            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<automation::IRuleManager> m_rulesManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
            static const std::string RestKeyword;
            static const std::string RestSubKeywordInterpreter;
            static const std::string RestSubKeywordRule;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
