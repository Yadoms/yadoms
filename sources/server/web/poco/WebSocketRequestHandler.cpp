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
#include "web/ws/DeviceDeletedFrame.h"
#include "web/ws/DeviceBlackListedFrame.h"
#include "web/ws/KeywordDeletedFrame.h"
#include "web/ws/KeywordNewFrame.h"
#include "web/ws/TaskUpdateNotificationFrame.h"
#include "web/ws/TimeNotificationFrame.h"
#include "web/ws/IsAliveFrame.h"

#include "notification/acquisition/Observer.hpp"
#include "notification/summary/Notification.hpp"
#include "notification/basic/Observer.hpp"
#include "notification/Helpers.hpp"
#include "notification/action/EventAction.hpp"

#include <shared/tools/Random.h>

namespace web
{
   namespace poco
   {
      CWebSocketRequestHandler::CWebSocketRequestHandler()
		  :m_wsIdDebug(shared::tools::CRandom::generateUUID())
      {
      }

      CWebSocketRequestHandler::~CWebSocketRequestHandler()
      {
      }

      void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                                   Poco::Net::HTTPServerResponse& response)
      {
         YADOMS_LOG_CONFIGURE("Websocket connection " + m_wsIdDebug);
         YADOMS_LOG(information) << "New websocket client";

         std::vector<boost::shared_ptr<notification::IObserver>> observers;

         boost::thread wsReceiverThread;
         auto eventHandler = boost::make_shared<shared::event::CEventHandler>();

         // For each request (each time a new ws connexion is made),
         // just create a websocket server and wait infinite (until client ends)
         try
         {
            // Subscribe to new acquisitions depends on filters set by GUI
            auto acquisitionAction(boost::make_shared<notification::action::CEventAction<notification::acquisition::CNotification>>(*eventHandler,
                                                                                                                                    kNewAcquisition));
            auto newAcquisitionObserver(boost::make_shared<notification::acquisition::CObserver>(acquisitionAction));
            notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);
            observers.push_back(newAcquisitionObserver);

            auto acquisitionSummaryAction(boost::make_shared<notification::action::CEventAction<notification::summary::CNotification>>(*eventHandler,
                                                                                                                                       kNewAcquisitionSummary));
            auto newAcquisitionSummaryObserver(boost::make_shared<notification::basic::CObserver<notification::summary::CNotification>>(acquisitionSummaryAction));
            notification::CHelpers::subscribeCustomObserver(newAcquisitionSummaryObserver);
            observers.push_back(newAcquisitionSummaryObserver);

            // Subscribe to new device notifications
            observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CDevice>(notification::change::EChangeType::kCreate,
                                                                                                             *eventHandler,
                                                                                                             kNewDevice));

            // Subscribe to device deletion notifications
            observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CDevice>(notification::change::EChangeType::kDelete,
                                                                                                             *eventHandler,
                                                                                                             kDeleteDevice));

            // Subscribe to device blacklist notifications
            observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CDevice>(notification::change::EChangeType::kBlacklist,
                                                                                                             *eventHandler,
                                                                                                             kBlackListDevice));

            // Subscribe to keyword creation notifications
            observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CKeyword>(notification::change::EChangeType::kCreate,
                                                                                                              *eventHandler,
                                                                                                              kNewKeyword));

            // Subscribe to keyword deletion notifications
            observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CKeyword>(notification::change::EChangeType::kDelete,
                                                                                                             *eventHandler,
                                                                                                             kDeleteKeyword));

            // Subscribe to event logger notifications
            observers.push_back(notification::CHelpers::subscribeBasicObserver<database::entities::CEventLogger>(*eventHandler,
                                                                                                                 kNewLogEvent));

            // Subscribe to task progression notifications
            observers.push_back(notification::CHelpers::subscribeBasicObserver<task::CInstanceNotificationData>(*eventHandler,
                                                                                                                kTaskProgression));

            // Ping timer
            eventHandler->createTimer(kPing,
                                      shared::event::CEventTimer::EPeriodicity::kOneShot,
                                      boost::posix_time::seconds(2));
            auto pongReceived = false;

            // Every minute timer
            eventHandler->createTimer(kEveryMinute,
                                      shared::event::CEventTimer::EPeriodicity::kPeriodic,
                                      boost::posix_time::minutes(1));

            auto webSocket = boost::make_shared<Poco::Net::WebSocket>(request,
                                                                      response);

            wsReceiverThread = boost::thread(&CWebSocketRequestHandler::receiverThreaded,
                                             eventHandler,
                                             webSocket);

            auto clientSeemConnected = true;

            // Everything is initialized, send a time event immediately
            clientSeemConnected = send(webSocket,
                                       ws::CTimeNotificationFrame(shared::currentTime::Provider().now()));

            while (clientSeemConnected)
            {
               //manage server send to websocket data
               switch (eventHandler->waitForEvents())
               {
               case kConnectionLost:
                  {
                     clientSeemConnected = false;
                     break;
                  }

               case kPing:
                  {
                     // Send ping
                     if (sendPing(webSocket))
                     {
                        pongReceived = false;
                        eventHandler->createTimer(kPongTimeout,
                                                  shared::event::CEventTimer::EPeriodicity::kOneShot,
                                                  boost::posix_time::seconds(2));
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "Fail to send PING frame";
                        // Retry
                        eventHandler->createTimer(kPing,
                                                  shared::event::CEventTimer::EPeriodicity::kOneShot,
                                                  boost::posix_time::seconds(2));
                     }
                     break;
                  }

               case kPongReceived:
                  {
                     pongReceived = true;
                     // Restart timer for next ping
                     eventHandler->createTimer(kPing,
                                               shared::event::CEventTimer::EPeriodicity::kOneShot,
                                               boost::posix_time::seconds(2));
                     break;
                  }

               case kPongTimeout:
                  {
                     if (!pongReceived)
                        throw shared::exception::CException("No answer to ping");
                     break;
                  }

               case kReceived:
                  {
                     auto parsedFrame = eventHandler->getEventData<boost::shared_ptr<ws::CFrameBase>>();
                     switch (parsedFrame->getType())
                     {
                     case ws::CFrameBase::EFrameType::kAcquisitionFilterValue:
                        {
						   YADOMS_LOG(information) << "Receive acquisition filter : " << parsedFrame->serialize();
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
                        YADOMS_LOG(warning) << "Received websocket frame with unsupported type : " << parsedFrame->getType();
                        break;
                     }
                     break;
                  }

               case kReceivedException:
                  {
                     auto exception = eventHandler->getEventData<Poco::Net::WebSocketException>();
                     switch (exception.code())
                     {
                     case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                        response.set("Sec-WebSocket-Version",
                                     Poco::Net::WebSocket::WEBSOCKET_VERSION);
                        // fallthrough
                     default:
                        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
                                                    exception.message());
                        response.setContentLength(0);
                        response.send();
                        break;
                     }

                     clientSeemConnected = false;
                     break;
                  }

               case kEveryMinute:
                  {
                     clientSeemConnected = send(webSocket, ws::CTimeNotificationFrame(shared::currentTime::Provider().now()));
                     break;
                  }
               case kNewAcquisition:
                  {
                     auto notif = eventHandler->getEventData<boost::shared_ptr<notification::acquisition::CNotification>>();
                     clientSeemConnected = send(webSocket, ws::CAcquisitionUpdateFrame(notif->getAcquisition()));
                     break;
                  }
               case kNewAcquisitionSummary:
                  {
                     auto notif = eventHandler->getEventData<boost::shared_ptr<notification::summary::CNotification>>();
                     clientSeemConnected = send(webSocket, ws::CAcquisitionSummaryUpdateFrame(notif->getAcquisitionSummaries()));
                     break;
                  }
               case kNewDevice:
                  {
                     auto newDevice = eventHandler->getEventData<boost::shared_ptr<database::entities::CDevice>>();
                     clientSeemConnected = send(webSocket, ws::CNewDeviceFrame(newDevice));
                     break;
                  }
               case kDeleteDevice:
               {
                  auto newDevice = eventHandler->getEventData<boost::shared_ptr<database::entities::CDevice>>();
                  clientSeemConnected = send(webSocket, ws::CDeviceDeletedFrame(newDevice));
                  break;
               }
               case kBlackListDevice:
               {
                  auto newDevice = eventHandler->getEventData<boost::shared_ptr<database::entities::CDevice>>();
                  clientSeemConnected = send(webSocket, ws::CDeviceBlackListedFrame(newDevice));
                  break;
               }
               case kNewKeyword:
               {
                  auto keyword = eventHandler->getEventData<boost::shared_ptr<database::entities::CKeyword>>();
                  clientSeemConnected = send(webSocket, ws::CKeywordNewFrame(keyword));
                  break;
               }
               case kDeleteKeyword:
                  {
                     auto keyword = eventHandler->getEventData<boost::shared_ptr<database::entities::CKeyword>>();
                     clientSeemConnected = send(webSocket, ws::CKeywordDeletedFrame(keyword));
                     break;
                  }
               case kNewLogEvent:
                  {
                     auto logEvent = eventHandler->getEventData<boost::shared_ptr<database::entities::CEventLogger>>();
                     clientSeemConnected = send(webSocket, ws::CLogEventFrame(logEvent));
                     break;
                  }

               case kTaskProgression:
                  {
                     auto taskProgression = eventHandler->getEventData<boost::shared_ptr<task::CInstanceNotificationData>>();
                     clientSeemConnected = send(webSocket, ws::CTaskUpdateNotificationFrame(taskProgression));
                     break;
                  }

               default:
                  YADOMS_LOG(error) << "Unknown message id " << eventHandler->getEventId();
                  break;
               } // switch
            } // while
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

         wsReceiverThread.interrupt();
         wsReceiverThread.timed_join(boost::posix_time::seconds(10));

         //unsubscribe observers
         for (const auto& observer : observers)
            notification::CHelpers::unsubscribeObserver(observer);

         YADOMS_LOG(information) << "Websocket client lost";
      }

      void CWebSocketRequestHandler::receiverThreaded(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                                      boost::shared_ptr<Poco::Net::WebSocket> webSocket)
      {
         YADOMS_LOG_CONFIGURE("Websocket receiver thread");
         YADOMS_LOG(debug) << "Started";

         try
         {
            char buffer[2048] = {0};
            auto flags = 0;

            while (true)
            {
               try
               {
                  if (webSocket->receiveFrame(buffer, sizeof(buffer), flags) == 0)
                  {
                     eventHandler->postEvent(kConnectionLost);
                     YADOMS_LOG(debug) << "Websocket connection lost";
                     return;
                  }

                  if (flags & Poco::Net::WebSocket::FRAME_OP_PONG)
                  {
                     eventHandler->postEvent(kPongReceived);
                     continue;
                  }

                  std::string bufferString(buffer);
                  memset(buffer, 0, sizeof(buffer)); // clean up the buffer

                  YADOMS_LOG(debug) << "Websocket receive data : " << bufferString;

                  auto parsedFrame = ws::CFrameFactory::tryParse(bufferString);
                  if (!parsedFrame)
                  {
                     // Log as Debug because user actions in browser may 'kill' websockets connections
                     // and provide bad json data (refresh, close page,....)
                     YADOMS_LOG(debug) << "Fail to parse received data";
                  }

                  eventHandler->postEvent(kReceived,
                                          parsedFrame);
               }
               catch (Poco::Net::WebSocketException& exception)
               {
                  eventHandler->postEvent(kReceivedException,
                                          exception);
                  // Log as Debug because user actions in browser may 'kill' websockets connections
                  // and provide bad json data (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket receiveFrame Poco::Exception : " << exception.displayText();
                  return;
               }
               catch (Poco::Exception& exception)
               {
                  eventHandler->postEvent(kConnectionLost);
                  // Log as Debug because user actions in browser may 'kill' websockets connections
                  // and provide bad json data (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket receiveFrame Poco::Exception : " << exception.displayText();
                  return;
               }
               catch (std::exception& exception)
               {
                  eventHandler->postEvent(kConnectionLost);
                  // Log as Debug because user actions in browser may 'kill' websockets connections
                  // and provide bad json data (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket receiveFrame std::exception : " << exception.what();
                  return;
               }
               catch (...)
               {
                  eventHandler->postEvent(kConnectionLost);
                  // Log as Debug because user actions in browser may 'kill' websockets connections
                  // and provide bad json data (refresh, close page,....)
                  YADOMS_LOG(debug) << "Websocket unknown exception : ";
                  return;
               }
            }
         }
         catch (boost::thread_interrupted&)
         {
         }

         eventHandler->postEvent(kConnectionLost);
         YADOMS_LOG(debug) << "Ended";
      }

      bool CWebSocketRequestHandler::send(boost::shared_ptr<Poco::Net::WebSocket> webSocket,
                                          const ws::CFrameBase& toSend)
      {
         auto dataString = toSend.serialize();
         return (webSocket->sendFrame(dataString.c_str(),
                                      dataString.length(),
                                      Poco::Net::WebSocket::FRAME_TEXT) != 0);
      }

      bool CWebSocketRequestHandler::sendPing(boost::shared_ptr<Poco::Net::WebSocket> webSocket)
      {
         std::string dataString = "Yadoms play PING";
         return (webSocket->sendFrame(dataString.c_str(),
                                      dataString.length(),
                                      Poco::Net::WebSocket::FRAME_FLAG_FIN | Poco::Net::WebSocket::FRAME_OP_PING) != 0);
      }
   }
} //namespace web::poco


