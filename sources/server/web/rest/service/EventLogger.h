#pragma once
#include "IRestService.h"
#include "dataAccessLayer/IEventLogger.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CEventLogger : public IRestService
         {
         public:
            explicit CEventLogger(boost::shared_ptr<dataAccessLayer::IEventLogger> dataProvider);
            virtual ~CEventLogger();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

         protected:
            shared::CDataContainer getEvents(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getLastEvent(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getEventsFrom(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getEventsRange(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer addEvent(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            boost::shared_ptr<dataAccessLayer::IEventLogger> m_dataProvider;
            std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


