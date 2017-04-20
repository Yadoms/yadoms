#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <Poco/Net/NetException.h>

#include <shared/Log.h>
#include <shared/event/EventHandler.hpp>

#include "web/ws/FrameFactory.h"
#include "web/ws/AcquisitionFilterFrame.h"
#include "web/ws/AcquisitionUpdateFrame.h"
#include "web/ws/AcquisitionSummaryUpdateFrame.h"
#include "web/ws/LogEventFrame.h"
#include "web/ws/NewDeviceFrame.h"
#include "web/ws/TaskUpdateNotificationFrame.h"
#include "web/ws/IsAliveFrame.h"

#include "notification/acquisition/Observer.hpp"
#include "notification/summary/Notification.hpp"
#include "notification/basic/Observer.hpp"
#include "notification/Helpers.hpp"
#include "notification/action/EventAction.hpp"


namespace web
{
   namespace poco
   {
      CWebSocketRequestHandler::CWebSocketRequestHandler()
      {
      }

      CWebSocketRequestHandler::~CWebSocketRequestHandler()
      {
      }


      void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         YADOMS_LOG_CONFIGURE("Websocket server")
         YADOMS_LOG(information) << "New websocket client";

         std::vector<boost::shared_ptr<notification::IObserver>> observers;

         //for each request (each time a new ws connexion is made)
         //then just create a websocket server and wait infinite (until client ends)
         try
         {
            // Supported events and handler
            enum
               {
                  kNotifFromWsClient = shared::event::kUserFirstId,
                  kNewAcquisition,
                  kNewAcquisitionSummary,
                  kNewDevice,
                  kNewLogEvent,
                  kTaskProgression
               };
            shared::event::CEventHandler eventHandler;

            // Subscribe to new acquisitions depends on filters set by GUI
            auto acquisitionAction(boost::make_shared<notification::action::CEventAction<notification::acquisition::CNotification>>(eventHandler, kNewAcquisition));
            auto newAcquisitionObserver(boost::make_shared<notification::acquisition::CObserver>(acquisitionAction));
            notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);
            observers.push_back(newAcquisitionObserver);

            auto acquisitionSummaryAction(boost::make_shared<notification::action::CEventAction<notification::summary::CNotification>>(eventHandler, kNewAcquisitionSummary));
            auto newAcquisitionSummaryObserver(boost::make_shared<notification::basic::CObserver<notification::summary::CNotification>>(acquisitionSummaryAction));
            notification::CHelpers::subscribeCustomObserver(newAcquisitionSummaryObserver);
            observers.push_back(newAcquisitionSummaryObserver);

            // Subscribe to new device notifications
            observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CDevice>(notification::change::EChangeType::kCreate, eventHandler, kNewDevice));

            // Subscribe to event logger notifications
            observers.push_back(notification::CHelpers::subscribeBasicObserver<database::entities::CEventLogger>(eventHandler, kNewLogEvent));

            // Subscribe to task progression notifications
            observers.push_back(notification::CHelpers::subscribeBasicObserver<task::CInstanceNotificationData>(eventHandler, kTaskProgression));

            Poco::Net::WebSocket webSocket(request, response);

            auto clientSeemConnected = true;

            char buffer[2048] = {0};
            auto flags = 0;
            auto pingWaitCount = 0;
            auto pingpongFlag = 0;

            while (clientSeemConnected)
            {
               try
               {
                  //manage server send to websocket data
                  switch (eventHandler.waitForEvents(boost::posix_time::milliseconds(50)))
                  {
                  case kNewAcquisition:
                     {
                        auto notif = eventHandler.getEventData<boost::shared_ptr<notification::acquisition::CNotification>>();
                        clientSeemConnected = send(webSocket, ws::CAcquisitionUpdateFrame(notif->getAcquisition()));
                        break;
                     }
                  case kNewAcquisitionSummary:
                     {
                        auto notif = eventHandler.getEventData<boost::shared_ptr<notification::summary::CNotification>>();
                        clientSeemConnected = send(webSocket, ws::CAcquisitionSummaryUpdateFrame(notif->getAcquisitionSummaries()));
                        break;
                     }

                  case kNewDevice:
                     {
                        auto newDevice = eventHandler.getEventData<boost::shared_ptr<database::entities::CDevice>>();
                        clientSeemConnected = send(webSocket, ws::CNewDeviceFrame(newDevice));
                        break;
                     }

                  case kNewLogEvent:
                     {
                        auto logEvent = eventHandler.getEventData<boost::shared_ptr<database::entities::CEventLogger>>();
                        clientSeemConnected = send(webSocket, ws::CLogEventFrame(logEvent));
                        break;
                     }

                  case kTaskProgression:
                     {
                        auto taskProgression = eventHandler.getEventData<boost::shared_ptr<task::CInstanceNotificationData>>();
                        clientSeemConnected = send(webSocket, ws::CTaskUpdateNotificationFrame(taskProgression));
                        break;
                     }

                  case shared::event::kTimeout:
                     break;

                  default:
                     YADOMS_LOG(error) << "Unknown message id " << eventHandler.getEventId();
                     break;
                  } // switch
               }
               catch (shared::exception::CException& ex)
               {
                  YADOMS_LOG(error) << "Websocket request handler exception : " << ex.what();
                  clientSeemConnected = false;
               }
               catch (std::exception& ex)
               {
                  YADOMS_LOG(error) << "Websocket request handler std exception : " << ex.what();
                  clientSeemConnected = false;
               }
               catch (...)
               {
                  YADOMS_LOG(error) << "Websocket request handler unknown exception";
                  clientSeemConnected = false;
               }

               //listen websocket incoming data
               try
               {
                  if (webSocket.poll(Poco::Timespan(0, 50 * 1000), Poco::Net::Socket::SELECT_READ))
                  {
                     auto n = webSocket.receiveFrame(buffer, sizeof(buffer), flags);
                     if (n > 0)
                     {
                        if ((flags & Poco::Net::WebSocket::FRAME_OP_PONG) == Poco::Net::WebSocket::FRAME_OP_PONG)
                        {
                           //we receive a pong frame, reset flag
                           pingpongFlag = 0;
                        }
                        else
                        {
                           std::string bufferString(buffer);

                           YADOMS_LOG(debug) << "Websocket receive data : " << bufferString;

                           auto parsedFrame = ws::CFrameFactory::tryParse(bufferString);
                           if (parsedFrame)
                           {
                              switch (parsedFrame->getType())
                              {
                              case ws::CFrameBase::EFrameType::kAcquisitionFilterValue:
                                 {
                                    auto parsedFrameAsqFilter = boost::dynamic_pointer_cast<ws::CAcquisitionFilterFrame>(parsedFrame);
                                    newAcquisitionObserver->resetKeywordIdFilter(parsedFrameAsqFilter->getFilter());
                                    break;
                                 }
                              case ws::CFrameBase::EFrameType::kIsAliveValue:
                                 {
                                    send(webSocket, ws::CIsAliveFrame());
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
                  clientSeemConnected = false;
               }
               catch (Poco::TimeoutException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::TimeoutException : " << exc.displayText();
                  clientSeemConnected = false;
               }
               catch (Poco::Net::NetException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::NetException : " << exc.displayText();
                  clientSeemConnected = false;
               }
               catch (Poco::IOException& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::IOException : " << exc.displayText();
                  clientSeemConnected = false;
               }
               catch (Poco::Exception& exc)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler Poco::Exception : " << exc.displayText();
                  clientSeemConnected = false;
               }
               catch (shared::exception::CException& ex)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler exception : " << ex.what();
                  clientSeemConnected = false;
               }
               catch (...)
               {
                  //log as Debug because : user actions in browser may 'kill' websockets connections (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket request handler unknown exception";
                  clientSeemConnected = false;
               }

               //the while take 100ms max (two 50ms timeout)
               if (pingWaitCount++ >= 20)
               {
                  pingWaitCount = 0;

                  //check flag value
                  if (pingpongFlag++ > 2)
                  {
                     YADOMS_LOG(debug) << "WebSocketclient don't answer to ping request";
                     clientSeemConnected = false;
                  }

                  //send ping
                  if (!sendPing(webSocket))
                  {
                     YADOMS_LOG(warning) << "Fail to send PING frame";
                  }
               }
            } //while
         }
         catch (shared::exception::CException& ex)
         {
            YADOMS_LOG(error) << "Websocket request handler exception : " << ex.what();
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Websocket request handler std exception : " << ex.what();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Websocket request handler unknown exception";
         }

         //unsubscribe observers
         for (const auto& observer : observers)
            notification::CHelpers::unsubscribeObserver(observer);
      }

      bool CWebSocketRequestHandler::send(Poco::Net::WebSocket& webSocket, const ws::CFrameBase& toSend)
      {
         auto dataString = toSend.serialize();
         return (webSocket.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT) != 0);
      }

      bool CWebSocketRequestHandler::sendPing(Poco::Net::WebSocket& webSocket)
      {
         std::string dataString = "Yadoms play PING";
         return (webSocket.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_FLAG_FIN | Poco::Net::WebSocket::FRAME_OP_PING) != 0);
      }
   }
} //namespace web::poco


