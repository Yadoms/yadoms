#pragma once
#include "IRestService.h"
#include "dataAccessLayer/IEventLogger.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CEventLogger final : public IRestService
         {
         public:
            explicit CEventLogger(boost::shared_ptr<dataAccessLayer::IEventLogger> dataProvider);
            ~CEventLogger() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         protected:
            boost::shared_ptr<shared::serialization::IDataSerializable> getEvents(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getLastEvent(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getEventsFrom(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getEventsRange(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> addEvent(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            boost::shared_ptr<dataAccessLayer::IEventLogger> m_dataProvider;
            std::string m_restKeyword;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


