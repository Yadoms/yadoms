#include "stdafx.h"
#include "WebsocketListener.h"

#include <utility>

#include "shared/Log.h"
#include "web/ws/FrameFactory.h"

namespace web
{
   namespace oatppServer
   {
      CWebsocketListener::CWebsocketListener(boost::shared_ptr<CWebsocketOnNewAcquisitionHandler> acquisitionObserver)
         : m_acquisitionObserver(std::move(acquisitionObserver))
      {
         YADOMS_LOG_CONFIGURE("Websocket")
      }

      void CWebsocketListener::onPing(const WebSocket& socket,
                                      const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onPing";
         socket.sendPong(message);
      }

      void CWebsocketListener::onPong(const WebSocket& socket,
                                      const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onPong";
      }

      void CWebsocketListener::onClose(const WebSocket& socket,
                                       v_uint16 code,
                                       const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onClose, code = " << code;
      }

      void CWebsocketListener::readMessage(const WebSocket& socket,
                                           v_uint8 opcode,
                                           p_char8 data,
                                           oatpp::v_io_size size)
      {
         if (size > 0)
         {
            // message frame received

            m_messageBuffer.writeSimple(data, static_cast<v_buff_size>(size));
            return;
         }

         if (size == 0)
         {
            // message transfer finished

            const auto wholeMessage = m_messageBuffer.toString();
            m_messageBuffer.setCurrentPosition(0);

            try
            {
               processReceivedMessage(wholeMessage,
                                      socket);
            }
            catch (const std::exception&)
            {
               YADOMS_LOG(error) << "Error processing received message " << wholeMessage->c_str();
            }
         }
      }

      void CWebsocketListener::sendMessage(const std::string& message,
                                           const WebSocket& socket)
      {
         //TODO gérer tous les send :
         // - acquisition summary
         // - IsAlive périodique
         // - timeNotification
         // - newDevice ==> à supprimer ?
         // - deviceDeleted ==> à supprimer ?
         // - newKeyword ==> à supprimer ?
         // - keywordDeleted ==> à supprimer ?
         // - eventLog
         // - taskUpdate ==> à supprimer ?
         socket.sendOneFrameText(message);
      }

      std::string CWebsocketListener::makeIsAliveReply()
      {
         shared::CDataContainer reply;
         reply.set("type", "isAlive");
         return reply.serialize();
      }

      void CWebsocketListener::processReceivedMessage(const std::string& receivedMessage,
                                                      const WebSocket& socket) const
      {
         const shared::CDataContainer frame(receivedMessage);
         const auto frameType = frame.get<std::string>("type");

         if (frameType == "isAlive")
         {
            static const auto IsAliveReply = makeIsAliveReply();
            sendMessage(IsAliveReply, socket);
            return;
         }

         if (frameType == "acquisitionFilter")
         {
            YADOMS_LOG(debug) << "Receive new acquisition filter : " << frame.getChild("keywords")->serialize();
            m_acquisitionObserver->setFilter(frame.get<std::vector<int>>("keywords"));
            return;
         }

         throw std::runtime_error("Invalid received message " + receivedMessage);
      }
   } //namespace oatppServer
} //namespace web
