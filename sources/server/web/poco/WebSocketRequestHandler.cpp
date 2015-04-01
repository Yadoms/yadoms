#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/event/EventHandler.hpp>

#include "../../notification/acquisition/observerAsEvent.h"
#include "../../notification/newDevice/observerAsEvent.h"
#include "../../notification/taskProgression/observerAsEvent.h"
#include "../../notification/observerSubscriber.hpp"

#include "web/ws/FrameFactory.h"
#include "web/ws/AcquisitionFilterFrame.h"
#include "web/ws/AcquisitionUpdateFrame.h"
#include "web/ws/NewDeviceFrame.h"
#include "web/ws/TaskUpdateNotificationFrame.h"


namespace web { namespace poco {

CWebSocketRequestHandler::CWebSocketRequestHandler(boost::shared_ptr<notification::INotificationCenter> notificationCenter)
   :m_notificationCenter(notificationCenter)
{
}

CWebSocketRequestHandler::~CWebSocketRequestHandler()
{
}


void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
   //for each request (each time a new ws connexion is made
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
      typedef notification::CObserverSubscriber<notification::acquisition::INotifier, notification::acquisition::IObserver> AcquisitionSubscriber;
      std::vector<boost::shared_ptr<AcquisitionSubscriber> > acquisitionsSubscribers;

      // Subscribe to new device notifications
      boost::shared_ptr<notification::newDevice::IObserver> newDeviceObserver(new notification::newDevice::CObserverAsEvent(eventHandler, kNewDevice));
      notification::CObserverSubscriber<notification::newDevice::INotifier, notification::newDevice::IObserver> newDeviceSubscriber(m_notificationCenter->newDeviceNotifier(), newDeviceObserver);
      
      // Subscribe to task progression notifications
      boost::shared_ptr<notification::taskProgression::IObserver> taskProgressionObserver(new notification::taskProgression::CObserverAsEvent(eventHandler, kTaskProgression));
      notification::CObserverSubscriber<notification::taskProgression::INotifier, notification::taskProgression::IObserver> taskProgressionSubscriber(m_notificationCenter->taskProgressionNotifier(), taskProgressionObserver);
      
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
               std::vector<int> keywordsToListen = parsedFrameAsqFilter->getFilter();
               acquisitionsSubscribers.clear();
               if (keywordsToListen.empty())
               {
                  // No filter, so send all acquisitions
                  boost::shared_ptr<notification::acquisition::IObserver> acquisitionObserver(new notification::acquisition::CObserverAsEvent(eventHandler, kNewAcquisition, notification::acquisition::IObserver::kAll, true));
                  acquisitionsSubscribers.push_back(boost::make_shared<AcquisitionSubscriber>(m_notificationCenter->acquisitionNotifier(), acquisitionObserver));
               }
               else
               {
                  // Filter is present, send only needed acquisitions
                  for (std::vector<int>::const_iterator it = keywordsToListen.begin(); it != keywordsToListen.end(); ++it)
                  {
                     boost::shared_ptr<notification::acquisition::IObserver> acquisitionObserver(new notification::acquisition::CObserverAsEvent(eventHandler, kNewAcquisition, *it, true));
                     acquisitionsSubscribers.push_back(boost::make_shared<AcquisitionSubscriber>(m_notificationCenter->acquisitionNotifier(), acquisitionObserver));
                  }
               }
               break;
            }

            break;
         }

         case kNewAcquisition:
         {
            typedef boost::shared_ptr<const database::entities::CAcquisition> SingleAcquisition;
            typedef boost::tuple<SingleAcquisition,
                  boost::shared_ptr<const database::entities::CAcquisitionSummary>,
                  boost::shared_ptr<const database::entities::CAcquisitionSummary> > FullAcquisition;

            SingleAcquisition acquisition;
            boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary;
            boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary;

            if (eventHandler.isEventType<SingleAcquisition>())
               acquisition = eventHandler.getEventData<SingleAcquisition>();
            else if (eventHandler.isEventType<FullAcquisition>())
               boost::tie(acquisition, dailySummary, hourlySummary) = eventHandler.getEventData<FullAcquisition>();
            else
            {
               YADOMS_LOG(error) << "CWebSocketRequestHandler::handleRequest : Invalid type for acquisition event";
               break;
            }

            clientSeemConnected = send(client, ws::CAcquisitionUpdateFrame(*acquisition, dailySummary, hourlySummary));

            break;
         }

         case kNewDevice:
         {
            boost::shared_ptr<const database::entities::CDevice> newDevice = eventHandler.getEventData<boost::shared_ptr<const database::entities::CDevice> >();
            clientSeemConnected = send(client, ws::CNewDeviceFrame(*newDevice));
            break;
         }

         case kTaskProgression:
            {
               boost::shared_ptr<const task::IInstance> taskProgression = eventHandler.getEventData<boost::shared_ptr<const task::IInstance> >();
               clientSeemConnected = send(client, ws::CTaskUpdateNotificationFrame(*taskProgression));
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
}

bool CWebSocketRequestHandler::send(CWebSocketClient& client, const ws::CFrameBase& toSend) const
{
   std::string dataString = toSend.serialize();
   return (client.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT) != 0);
}

} } //namespace web::poco

