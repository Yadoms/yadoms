#include "stdafx.h"
#include "WebSocketConnection.h"
#include "WebsocketListener.h"
#include "notification/Helpers.hpp"
#include "notification/acquisition/Observer.hpp"

#include "shared/Log.h"
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
         kReceived
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

         m_connectionThread.interrupt();
         m_connectionThread.try_join_for(boost::chrono::seconds(2));

         //TODO voir pourquoi �a crashe quand une socket est connect�e � la fermeture de Yadoms
      }

      void CWebSocketConnection::sendMessage(const std::string& message,
                                             const WebSocket& socket)
      {
         //TODO gérer tous les send :
         // - acquisition summary
         // - IsAlive périodique
         // - timeNotification
         // - newDevice ==> à conserver (pour maj async de la liste des devices lors de la création d'un plugin par exemple)
         // - deviceDeleted ==> à conserver (pour maj des widgets après suppression device)
         // - newKeyword ==> à conserver (pour maj async de la liste des devices lors de la création d'un plugin par exemple)
         // - keywordDeleted ==> à conserver (pour maj des widgets après suppression keyword)
         // - deviceblacklisted ==> à supprimer ? Cette opération devrait être synchrone
         // - eventLog
         // - taskUpdate ==> à supprimer ?
         socket.sendOneFrameText(message);
      }

      std::unique_ptr<shared::CDataContainer> CWebSocketConnection::makeNewAcquisitionContainer() const
      {
         auto container = std::make_unique<shared::CDataContainer>();
         container->set("type", "newAcquisition");
         return container;
      }

      std::string CWebSocketConnection::makeIsAliveReply()
      {
         shared::CDataContainer reply;
         reply.set("type", "isAlive");
         return reply.serialize();
      }

      void CWebSocketConnection::handleConnectionThread(const oatpp::websocket::WebSocket& socket)
      {
         YADOMS_LOG_CONFIGURE("New Websocket connection " + shared::tools::CRandom::generateUUID())

         std::vector<boost::shared_ptr<notification::IObserver>> observers;

         auto acquisitionAction(boost::make_shared<notification::action::CEventAction<notification::acquisition::CNotification>>(
            m_eventHandler,
            kNewAcquisition));
         const auto newAcquisitionObserver(boost::make_shared<notification::acquisition::CObserver>(acquisitionAction));
         notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);
         observers.emplace_back(newAcquisitionObserver);

         // Ping timer
         const auto pingTimer = m_eventHandler.createTimer(kPingTimer,
                                                           shared::event::CEventTimer::EPeriodicity::kOneShot,
                                                           boost::posix_time::seconds(2));
         // Wait for pong timer
         const auto pongTimeoutTimer = m_eventHandler.createTimer(kPongTimeout,
                                                                  shared::event::CEventTimer::EPeriodicity::kOneShot);

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
                     throw boost::thread_interrupted();
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

                     shared::CDataContainer newAcquisitionContainer;
                     newAcquisitionContainer.set("newAcquisition", *newAcquisition);
                     sendMessage(newAcquisitionContainer.serialize(),
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
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "Invalid received message : " << m_eventHandler.getEventData<const std::string>();
                     }
                     break;
                  }
                  //TODO tous les autres case, et un default !
               }
            }
            catch (const boost::thread_interrupted&)
            {
               socket.sendClose();
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
