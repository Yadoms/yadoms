#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "dateTime/TimeZoneDatabase.h"

#include "notification/IObserver.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CServerSentEvents final : public IRestService
         {
         public:
            explicit CServerSentEvents(boost::shared_ptr<database::IDataProvider> dataProvider);
            ~CServerSentEvents() override = default;


            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            boost::shared_ptr<IAnswer> requestSseAcquisitions(const boost::shared_ptr<IRequest>& request);

            boost::shared_ptr<database::IDataProvider> m_dataProvider;

            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;

            //std::vector<boost::shared_ptr<notification::IObserver>> observers; //TODO utile ?
         };
      } //namespace service
   } //namespace rest
} //namespace web 
