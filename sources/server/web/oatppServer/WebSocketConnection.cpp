#include "stdafx.h"
#include "WebSocketConnection.h"
#include "WebsocketListener.h"
#include "notification/Helpers.hpp"
#include "notification/acquisition/Observer.hpp"
#include "notification/summary/Notification.hpp"
#include "notification/summary/Observer.hpp"

#include "shared/Log.h"
#include "shared/dateTime/Helper.h"
#include "shared/tools/Random.h"

namespace web
{
   namespace oatppServer
   {
      std::atomic<v_int32> CWebSocketConnection::m_clientsCount(0);

      enum WebSocketEvent
      {
         kPingTimer = shared::event::kUserFirstId,
         kOnPong,
         kPongTimeout,
         kOnPing,
         kNewAcquisition,
         kNewAcquisitionSummary,
         kDeviceCreated,
         kDeviceDeleted,
         kKeywordCreated,
         kKeywordDeleted,
         kReceived,
         kTimeSynchronization
      };

      void CWebSocketConnection::onAfterCreate(const oatpp::websocket::WebSocket& socket,
                                               const std::shared_ptr<const ParameterMap>& params)
      {
         ++m_clientsCount;
         YADOMS_LOG(information) << "New websocket connection (Client count=" << m_clientsCount.load() << ")";

         /* In this particular case we create one CWebsocketListener per each connection */
         /* Which may be redundant in many cases */
         socket.setListener(std::make_shared<CWebsocketListener>(m_eventHandler,
                                                                 kOnPong,
                                                                 kOnPing,
                                                                 kReceived));

         m_connectionThread = boost::thread([this, &socket]
         {
            handleConnectionThread(socket);
         });
      }

      void CWebSocketConnection::onBeforeDestroy(const oatpp::websocket::WebSocket& socket)
      {
         --m_clientsCount;
         YADOMS_LOG(information) << "Connection closed (Client count=" << m_clientsCount.load() << ")";

         close();
      }

      void CWebSocketConnection::close()
      {
         if (!m_connectionThread.joinable())
            return;
         m_connectionThread.interrupt();
         m_connectionThread.try_join_for(boost::chrono::seconds(2));
      }

      void CWebSocketConnection::sendMessage(const std::string& message,
                                             const WebSocket& socket)
      {
         //TODO gérer tous les send :
         // - eventLog ==> A remettre en place lorsque l'EventLogger aura été refactoré
         // - taskUpdate ==> à supprimer ? Ou à refactorer pour que le reload d'une page permette de réafficher l'état d'une tâche
         try
         {
            socket.sendOneFrameText(message);
         }
         catch (const std::runtime_error& exception)
         {
            YADOMS_LOG(error) << "Error posting on websocket : " << exception.what();
         }
      }

      void CWebSocketConnection::sendTimeSynchronization(const boost::posix_time::ptime& time,
                                                         const WebSocket& socket)
      {
         shared::CDataContainer container;
         container.set("serverCurrentTime", time);
         sendMessage(container.serialize(),
                     socket);
      }

      void CWebSocketConnection::sendNewAcquisition(const boost::shared_ptr<database::entities::CAcquisition>& newAcquisition,
                                                    const WebSocket& socket)
      {
         shared::CDataContainer container;
         container.set("newAcquisition", *newAcquisition);
         sendMessage(container.serialize(),
                     socket);
      }

      void CWebSocketConnection::sendDeviceCreated(const boost::shared_ptr<database::entities::CDevice>& device,
                                                   const WebSocket& socket)
      {
         shared::CDataContainer container;
         container.set("deviceCreated", device);
         sendMessage(container.serialize(),
                     socket);
      }

      void CWebSocketConnection::sendDeviceDeleted(const boost::shared_ptr<database::entities::CDevice>& device,
                                                   const WebSocket& socket)
      {
         shared::CDataContainer container;
         container.set("deviceDeleted", device);
         sendMessage(container.serialize(),
                     socket);
      }

      void CWebSocketConnection::sendKeywordCreated(const boost::shared_ptr<database::entities::CKeyword>& keyword,
                                                    const WebSocket& socket)
      {
         shared::CDataContainer container;
         container.set("keywordCreated", keyword);
         sendMessage(container.serialize(),
                     socket);
      }

      void CWebSocketConnection::sendKeywordDeleted(const boost::shared_ptr<database::entities::CKeyword>& keyword,
                                                    const WebSocket& socket)
      {
         shared::CDataContainer container;
         container.set("keywordDeleted", keyword);
         sendMessage(container.serialize(),
                     socket);
      }

      void CWebSocketConnection::handleConnectionThread(const oatpp::websocket::WebSocket& socket)
      {
         YADOMS_LOG_CONFIGURE("New Websocket connection " + shared::tools::CRandom::generateUUID())

         std::vector<boost::shared_ptr<notification::IObserver>> observers;

         // Subscriptions
         // - new acquisitions
         auto acquisitionAction(boost::make_shared<notification::action::CEventAction<notification::acquisition::CNotification>>(
            m_eventHandler,
            kNewAcquisition));
         const auto newAcquisitionObserver(boost::make_shared<notification::acquisition::CObserver>(acquisitionAction));
         notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);
         observers.emplace_back(newAcquisitionObserver);
         // - new acquisition summaries
         const auto newAcquisitionSummaryObserver(boost::make_shared<notification::summary::CObserver>(
            boost::make_shared<notification::action::CEventAction<notification::summary::CNotification>>(
               m_eventHandler,
               kNewAcquisitionSummary)));
         notification::CHelpers::subscribeCustomObserver(newAcquisitionSummaryObserver);
         observers.emplace_back(newAcquisitionSummaryObserver);
         // - devices
         observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CDevice>(notification::change::EChangeType::kCreate,
            m_eventHandler,
            kDeviceCreated));
         observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CDevice>(notification::change::EChangeType::kDelete,
            m_eventHandler,
            kDeviceDeleted));
         // - keywords
         observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CKeyword>(notification::change::EChangeType::kCreate,
            m_eventHandler,
            kKeywordCreated));
         observers.push_back(notification::CHelpers::subscribeChangeObserver<database::entities::CKeyword>(notification::change::EChangeType::kDelete,
            m_eventHandler,
            kKeywordDeleted));

         // Ping timer
         const auto pingTimer = m_eventHandler.createTimer(kPingTimer,
                                                           shared::event::CEventTimer::EPeriodicity::kOneShot,
                                                           boost::posix_time::seconds(2));
         // Wait for pong timer
         const auto pongTimeoutTimer = m_eventHandler.createTimer(kPongTimeout,
                                                                  shared::event::CEventTimer::EPeriodicity::kOneShot);

         // Time synchronization timer
         {
            const auto now = shared::currentTime::Provider().now();
            if (shared::dateTime::CHelper::nextMinuteOf(now) - now > boost::posix_time::seconds(2))
               sendTimeSynchronization(now,
                                       socket);
            m_eventHandler.createTimePoint(kTimeSynchronization,
                                           shared::dateTime::CHelper::nextMinuteOf(now));
         }


         while (true)
         {
            try
            {
               switch (m_eventHandler.waitForEvents())
               {
               case kPingTimer:
                  {
                     socket.sendPing(std::string());
                     pongTimeoutTimer->start(boost::posix_time::seconds(2));

                     break;
                  }
               case kOnPong:
                  {
                     pongTimeoutTimer->stop();
                     pingTimer->start();

                     break;
                  }
               case kPongTimeout:
                  {
                     YADOMS_LOG(error) << "No answer to ping";
                     throw boost::thread_interrupted(); // NOLINT(hicpp-exception-baseclass)
                  }
               case kOnPing:
                  {
                     socket.sendPong(std::string());
                     break;
                  }

               case kNewAcquisition:
                  {
                     auto newAcquisition = m_eventHandler.getEventData<boost::shared_ptr<notification::acquisition::CNotification>>()->
                                                          getAcquisition();
                     sendNewAcquisition(newAcquisition,
                                        socket);
                     break;
                  }
               case kDeviceCreated:
                  {
                     auto device = m_eventHandler.getEventData<boost::shared_ptr<database::entities::CDevice>>();
                     sendDeviceCreated(device,
                                       socket);
                     break;
                  }
               case kDeviceDeleted:
                  {
                     auto device = m_eventHandler.getEventData<boost::shared_ptr<database::entities::CDevice>>();
                     sendDeviceDeleted(device,
                                       socket);
                     break;
                  }
               case kKeywordCreated:
                  {
                     auto keyword = m_eventHandler.getEventData<boost::shared_ptr<database::entities::CKeyword>>();
                     sendKeywordCreated(keyword,
                                        socket);
                     break;
                  }
               case kKeywordDeleted:
                  {
                     auto keyword = m_eventHandler.getEventData<boost::shared_ptr<database::entities::CKeyword>>();
                     sendKeywordDeleted(keyword,
                                        socket);
                     break;
                  }
               case kNewAcquisitionSummary:
                  {
                     auto newSummary = m_eventHandler.getEventData<boost::shared_ptr<notification::summary::CNotification>>()->
                                                      getAcquisitionSummaries();

                     shared::CDataContainer newAcquisitionSummaryContainer;
                     newAcquisitionSummaryContainer.set("newAcquisitionSummary", newSummary);
                     sendMessage(newAcquisitionSummaryContainer.serialize(),
                                 socket);
                     break;
                  }

               case kReceived:
                  {
                     const shared::CDataContainer frame(m_eventHandler.getEventData<const std::string>());

                     if (frame.exists("acquisitionFilter"))
                     {
                        YADOMS_LOG(debug) << "Receive new acquisition filter : " << frame.getChild("acquisitionFilter.keywords")->serialize();
                        newAcquisitionObserver->resetKeywordIdFilter(frame.get<std::vector<int>>("acquisitionFilter.keywords"));
                        newAcquisitionSummaryObserver->resetKeywordIdFilter(frame.get<std::vector<int>>("acquisitionFilter.keywords"));
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "Invalid received message : " << m_eventHandler.getEventData<const std::string>();
                     }
                     break;
                  }
               case kTimeSynchronization:
                  {
                     const auto now = shared::currentTime::Provider().now();
                     sendTimeSynchronization(now,
                                             socket);
                     m_eventHandler.createTimePoint(kTimeSynchronization,
                                                    shared::dateTime::CHelper::nextMinuteOf(now));
                     break;
                  }
               default:
                  {
                     YADOMS_LOG(error) << "Invalid received event : " << m_eventHandler.getEventId() << ", ignored";
                     break;
                  }
               }
            }
            catch (const boost::thread_interrupted&)
            {
               socket.sendClose();
               socket.stopListening();
               break;
            }
            catch (const std::exception& e)
            {
               YADOMS_LOG(error) << "Error in socket connection handling, " << e.what();
            }
         }

         for (const auto& observer : observers)
            notification::CHelpers::unsubscribeObserver(observer);

         YADOMS_LOG(information) << "Websocket client lost";
      }
   } //namespace oatppServer
} //namespace web
