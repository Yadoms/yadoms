#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/event/EventHandler.hpp>
#include <shared/ServiceLocator.h>

#include "web/ws/FrameFactory.h"
#include "web/ws/AcquisitionFilterFrame.h"
#include "web/ws/AcquisitionUpdateFrame.h"
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
   //TODO : notification : refaire les enregistrements

   boost::shared_ptr< notification::IObserver > newDeviceObserver;
   boost::shared_ptr< notification::IObserver > newProgressionObserver;
   boost::shared_ptr< notification::acquisition::CObserver > newAcquisitionObserver;

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
         kTaskProgression
      };
      shared::event::CEventHandler eventHandler;


      // Subscribe to new acquisitions depends on filters set by GUI
      boost::shared_ptr< notification::action::CEventAction< notification::acquisition::CNotification > > acquisitionAction(new notification::action::CEventAction<notification::acquisition::CNotification>(eventHandler, kNewAcquisition));
      newAcquisitionObserver.reset(new notification::acquisition::CObserver(acquisitionAction));
      notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);

      // Subscribe to new device notifications
      newDeviceObserver = notification::CHelpers::subscribeChangeObserver< database::entities::CDevice >(notification::change::EChangeType::kCreate, eventHandler, kNewDevice);

      // Subscribe to task progression notifications
      newProgressionObserver = notification::CHelpers::subscribeBasicObserver< task::IInstance >(eventHandler, kTaskProgression);

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
            boost::shared_ptr<const database::entities::CDevice> newDevice = eventHandler.getEventData<boost::shared_ptr<const database::entities::CDevice> >();
            clientSeemConnected = send(client, ws::CNewDeviceFrame(newDevice));
            break;
         }

         case kTaskProgression:
         {
            boost::shared_ptr<const task::IInstance> taskProgression = eventHandler.getEventData<boost::shared_ptr<const task::IInstance> >();
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
   catch (...)
   {
      YADOMS_LOG(error) << "Websocket request handler unknown exception";
   }

   //unsubscribe observers
   if (newDeviceObserver)
      notification::CHelpers::unsubscribeObserver(newDeviceObserver);
   if (newProgressionObserver)
      notification::CHelpers::unsubscribeObserver(newProgressionObserver);
   if (newAcquisitionObserver)
      notification::CHelpers::unsubscribeObserver(newAcquisitionObserver);

}

bool CWebSocketRequestHandler::send(CWebSocketClient& client, const ws::CFrameBase& toSend) const
{
   std::string dataString = toSend.serialize();
   return (client.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT) != 0);
}

} } //namespace web::poco

