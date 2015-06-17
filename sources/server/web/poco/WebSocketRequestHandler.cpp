#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/event/EventHandler.hpp>
#include <shared/ServiceLocator.h>

#include "web/ws/FrameFactory.h"
#include "web/ws/AcquisitionFilterFrame.h"
#include "web/ws/AcquisitionUpdateFrame.h"
#include "web/ws/LogEventFrame.h"
#include "web/ws/NewDeviceFrame.h"
#include "web/ws/TaskUpdateNotificationFrame.h"

#include "notification/acquisition/Observer.hpp"
#include "notification/change/Observer.hpp"
#include "notification/basic/Observer.hpp"
#include "notification/Helpers.hpp"
#include "notification/action/EventAction.hpp"

namespace web { namespace poco {

CWebSocketRequestHandler::CWebSocketRequestHandler()
{
}

CWebSocketRequestHandler::~CWebSocketRequestHandler()
{
}


void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
   YADOMS_LOG(information) << "New websocket client";

   std::vector<boost::shared_ptr<notification::IObserver> > observers;

   //for each request (each time a new ws connexion is made)
   //then just create a websocket server and wait infinite (until client ends)
   try
   {
      // Supported events and handler
      enum
      {
         kNotifFromWsClient = shared::event::kUserFirstId,
         kNewAcquisition,
         kNewDevice,
         kNewLogEvent,
         kTaskProgression
      };
      shared::event::CEventHandler eventHandler;


      // Subscribe to new acquisitions depends on filters set by GUI
      boost::shared_ptr< notification::action::CEventAction< notification::acquisition::CNotification > > acquisitionAction(new notification::action::CEventAction<notification::acquisition::CNotification>(eventHandler, kNewAcquisition));
      boost::shared_ptr< notification::acquisition::CObserver > newAcquisitionObserver(new notification::acquisition::CObserver(acquisitionAction));
      notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);
      observers.push_back(newAcquisitionObserver);

      // Subscribe to new device notifications
      observers.push_back(notification::CHelpers::subscribeChangeObserver< database::entities::CDevice >(notification::change::EChangeType::kCreate, eventHandler, kNewDevice));

      // Subscribe to event logger notifications
      observers.push_back(notification::CHelpers::subscribeBasicObserver< database::entities::CEventLogger >(eventHandler, kNewLogEvent));

      // Subscribe to task progression notifications
      observers.push_back(notification::CHelpers::subscribeBasicObserver< task::CInstanceNotificationData >(eventHandler, kTaskProgression));

      // The web scoket client
      CWebSocketClient client(request, response, eventHandler, kNotifFromWsClient);
      client.start();

      bool clientSeemConnected = true;
      while (clientSeemConnected)
      {
         switch (eventHandler.waitForEvents())
         {
         case kNotifFromWsClient:
         {
            boost::shared_ptr<ws::CFrameBase> parsedFrame = eventHandler.getEventData<boost::shared_ptr<ws::CFrameBase> >();
            switch (parsedFrame->getType())
            {
            case ws::CFrameBase::EFrameType::kAcquisitionFilterValue:
               boost::shared_ptr<ws::CAcquisitionFilterFrame> parsedFrameAsqFilter = boost::dynamic_pointer_cast<ws::CAcquisitionFilterFrame>(parsedFrame);
               newAcquisitionObserver->resetKeywordIdFilter(parsedFrameAsqFilter->getFilter());
               break;
            }

            break;
         }

         case kNewAcquisition:
         {
            boost::shared_ptr<notification::acquisition::CNotification> notif = eventHandler.getEventData< boost::shared_ptr<notification::acquisition::CNotification> >();
            clientSeemConnected = send(client, ws::CAcquisitionUpdateFrame(notif->getAcquisition(), notif->getDailyAcquisition(), notif->getHourlyAcquisition()));
            break;
         }

         case kNewDevice:
         {
            boost::shared_ptr<database::entities::CDevice> newDevice = eventHandler.getEventData<boost::shared_ptr<database::entities::CDevice> >();
            clientSeemConnected = send(client, ws::CNewDeviceFrame(newDevice));
            break;
         }

         case kNewLogEvent:
         {
            boost::shared_ptr<database::entities::CEventLogger> logEvent = eventHandler.getEventData<boost::shared_ptr<database::entities::CEventLogger> >();
            clientSeemConnected = send(client, ws::CLogEventFrame(logEvent));
            break;
         }

         case kTaskProgression:
         {
            boost::shared_ptr<task::CInstanceNotificationData> taskProgression = eventHandler.getEventData<boost::shared_ptr<task::CInstanceNotificationData> >();
            clientSeemConnected = send(client, ws::CTaskUpdateNotificationFrame(taskProgression));
            break;
         }

         default:
            YADOMS_LOG(error) << "Unknown message id";
            break;
         }
      } //while
   }
   catch (shared::exception::CException & ex)
   {
      YADOMS_LOG(error) << "Websocket request handler exception : " << ex.what();
   }
   catch (std::exception & ex)
   {
      YADOMS_LOG(error) << "Websocket request handler std exception : " << ex.what();
   }  
   catch (...)
   {
      YADOMS_LOG(error) << "Websocket request handler unknown exception";
   }

   //unsubscribe observers
   for (std::vector<boost::shared_ptr<notification::IObserver> >::iterator observer = observers.begin(); observer != observers.end(); ++observer)
      notification::CHelpers::unsubscribeObserver(*observer);
}

bool CWebSocketRequestHandler::send(CWebSocketClient& client, const ws::CFrameBase& toSend) const
{
   std::string dataString = toSend.serialize();
   return (client.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT) != 0);
}

} } //namespace web::poco

