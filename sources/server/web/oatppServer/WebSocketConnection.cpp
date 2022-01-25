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
         kNewAcquisition = shared::event::kUserFirstId,
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

         try
         {
            auto acquisitionAction(boost::make_shared<notification::action::CEventAction<notification::acquisition::CNotification>>(
               m_eventHandler,
               kNewAcquisition));
            const auto newAcquisitionObserver(boost::make_shared<notification::acquisition::CObserver>(acquisitionAction));
            notification::CHelpers::subscribeCustomObserver(newAcquisitionObserver);
            observers.emplace_back(newAcquisitionObserver);

            while (true)
            {
               switch (m_eventHandler.waitForEvents())
               {
               case kNewAcquisition:
                  {
                     auto newAcquisition = m_eventHandler.getEventData<boost::shared_ptr<notification::acquisition::CNotification>>()->
                                                          getAcquisition();

                     static auto newAcquisitionContainer = makeNewAcquisitionContainer();
                     newAcquisitionContainer->set("data", *newAcquisition);
                     sendMessage(newAcquisitionContainer->serialize(),
                                 socket);
                     break;
                  }

               case kReceived:
                  {
                     const auto receivedMessage = m_eventHandler.getEventData<const std::string>();

                     const shared::CDataContainer frame(receivedMessage);
                     const auto frameType = frame.get<std::string>("type");

                     if (frameType == "isAlive")
                     {
                        static const auto IsAliveReply = makeIsAliveReply();
                        sendMessage(IsAliveReply, socket);
                     }
                     else if (frameType == "acquisitionFilter")
                     {
                        YADOMS_LOG(debug) << "Receive new acquisition filter : " << frame.getChild("keywords")->serialize();
                        newAcquisitionObserver->resetKeywordIdFilter(frame.get<std::vector<int>>("keywords"));
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "Invalid received message : " << receivedMessage;
                     }
                     break;
                  }
                  //TODO tous les autres case, et un default !
               }
            }
         }
         catch (const boost::thread_interrupted&)
         {
         }
         catch (const std::exception& e)
         {
            YADOMS_LOG(error) << "Error in socket connection handling, " << e.what();
         }

         for (const auto& observer : observers)
            notification::CHelpers::unsubscribeObserver(observer);

         YADOMS_LOG(information) << "Websocket client lost";
      }
   } //namespace oatppServer
} //namespace web
