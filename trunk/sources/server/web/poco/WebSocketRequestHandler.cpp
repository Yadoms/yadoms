#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <Poco/Net/NetException.h>

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

namespace web {
   namespace poco {

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

            Poco::Net::WebSocket webSocket(request, response);

            bool clientSeemConnected = true;

            char buffer[2048] = { 0 };
            int flags = 0;

            while (clientSeemConnected)
            {
               try
               {
                  //manage server send to websocket data
                  switch (eventHandler.waitForEvents(boost::posix_time::milliseconds(50)))
                  {

                  case kNewAcquisition:
                  {
                     boost::shared_ptr<notification::acquisition::CNotification> notif = eventHandler.getEventData< boost::shared_ptr<notification::acquisition::CNotification> >();
                     clientSeemConnected = send(webSocket, ws::CAcquisitionUpdateFrame(notif->getAcquisition(), notif->getDailyAcquisition(), notif->getHourlyAcquisition()));
                     break;
                  }

                  case kNewDevice:
                  {
                     boost::shared_ptr<database::entities::CDevice> newDevice = eventHandler.getEventData<boost::shared_ptr<database::entities::CDevice> >();
                     clientSeemConnected = send(webSocket, ws::CNewDeviceFrame(newDevice));
                     break;
                  }

                  case kNewLogEvent:
                  {
                     boost::shared_ptr<database::entities::CEventLogger> logEvent = eventHandler.getEventData<boost::shared_ptr<database::entities::CEventLogger> >();
                     clientSeemConnected = send(webSocket, ws::CLogEventFrame(logEvent));
                     break;
                  }

                  case kTaskProgression:
                  {
                     boost::shared_ptr<task::CInstanceNotificationData> taskProgression = eventHandler.getEventData<boost::shared_ptr<task::CInstanceNotificationData> >();
                     clientSeemConnected = send(webSocket, ws::CTaskUpdateNotificationFrame(taskProgression));
                     break;
                  }

                  case shared::event::kTimeout:
                     break;

                  default:
                     YADOMS_LOG(error) << "Unknown message id";
                     break;
                  } // switch
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

               //listen websocket incoming data
               try
               {
                  if (webSocket.poll(Poco::Timespan(0, 50 * 1000), Poco::Net::Socket::SELECT_READ))
                  {
                     int n = webSocket.receiveFrame(buffer, sizeof(buffer), flags);
                     if (n > 0)
                     {
                        std::string bufferString(buffer);

                        YADOMS_LOG(debug) << "Websocket receive data : " << bufferString;

                        boost::shared_ptr<ws::CFrameBase> parsedFrame = ws::CFrameFactory::tryParse(bufferString);
                        if (parsedFrame)
                        {
                           switch (parsedFrame->getType())
                           {
                           case ws::CFrameBase::EFrameType::kAcquisitionFilterValue:
                           {
                              boost::shared_ptr<ws::CAcquisitionFilterFrame> parsedFrameAsqFilter = boost::dynamic_pointer_cast<ws::CAcquisitionFilterFrame>(parsedFrame);
                              newAcquisitionObserver->resetKeywordIdFilter(parsedFrameAsqFilter->getFilter());
                              break;
                           }
                           default:
                              YADOMS_LOG(debug) << "Unmanaged websocket frame from client";
                              break;
                           }
                        }
                        else
                        {
                           //log as Debug because : user actions in browser may 'kill' websockets connections and provide bad json data (refresh, close page,....)
                           YADOMS_LOG(debug) << "Fail to parse received data.";
                        }
                     }
                  }
               }
               catch (Poco::Net::WebSocketException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::Net::WebSocketException";
                  switch (exc.code())
                  {
                  case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                     response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
                     // fallthrough
                  default:
                     response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, exc.message());
                     response.setContentLength(0);
                     response.send();
                     break;
                  }
               }
               catch (Poco::TimeoutException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::TimeoutException : " << exc.displayText();
               }
               catch (Poco::Net::NetException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::NetException : " << exc.displayText();
               }
               catch (Poco::IOException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::IOException : " << exc.displayText();
               }
               catch (Poco::Exception& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::Exception : " << exc.displayText();
               }
               catch (shared::exception::CException & ex)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler exception : " << ex.what();
               }
               catch (...)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler unknown exception";
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

      bool CWebSocketRequestHandler::send(Poco::Net::WebSocket & webSocket, const ws::CFrameBase& toSend) const
      {
         std::string dataString = toSend.serialize();
         return (webSocket.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT) != 0);
      }

   }
} //namespace web::poco

