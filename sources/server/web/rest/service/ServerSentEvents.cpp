#include "ServerSentEvents.h"

#include "RestEndPoint.h"
#include "stdafx.h"
#include "notification/Helpers.hpp"
#include "notification/IObserver.h"
#include "notification/acquisition/Notification.hpp"
#include "notification/acquisition/Observer.hpp"
#include "notification/action/EventAction.hpp"
#include "shared/exception/EmptyResult.hpp"
#include "web/rest/CreatedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CServerSentEvents::CServerSentEvents(boost::shared_ptr<database::IDataProvider> dataProvider)
            : m_dataProvider(std::move(dataProvider))
         {
         }

         void CServerSentEvents::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            //TODO à supprimer
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CServerSentEvents::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "sse/acquisitions/{keywords}", requestSseAcquisitions));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CServerSentEvents::requestSseAcquisitions(const boost::shared_ptr<IRequest>& request)
         {
            try
            {
               const auto keywordIds = request->pathVariableExists("keywords")
                                          ? CHelpers::convertToIntSet(request->pathVariableAsList("keywords"))
                                          : std::make_unique<std::set<int>>();
               if (keywordIds->empty())
                  throw std::invalid_argument("No keyword provided");


               // Subscriptions
               // - new acquisitions
               const auto eventHandler = boost::make_shared<shared::event::CEventHandler>();
               enum { SseEventId = shared::event::kUserFirstId };
               auto acquisitionAction = boost::make_shared<notification::action::CEventPtrAction<notification::acquisition::CNotification>>(
                  eventHandler,
                  SseEventId);
               const auto newAcquisitionObserver = boost::make_shared<notification::acquisition::CObserver>(acquisitionAction);
               notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver); //TODO unsubscribe quelque part ?
               //observers.emplace_back(newAcquisitionObserver);

               return boost::make_shared<CSuccessAnswer>(eventHandler,
                                                         SseEventId);
            }

            catch (const std::invalid_argument& arg)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                       arg.what());
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available plugins");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
