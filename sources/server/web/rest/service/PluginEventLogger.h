#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPluginEventLogger final : public IRestService
         {
         public:
            explicit CPluginEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider);
            ~CPluginEventLogger() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

            const std::string& getRestKeyword() const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getLogsForPluginName(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const;

         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            const std::string m_restKeyword;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
