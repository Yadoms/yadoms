#include "stdafx.h"
#include "WebsocketListener.h"

#include "shared/Log.h"
#include "web/ws/FrameFactory.h"

namespace web
{
   namespace oatppServer
   {
      CWebsocketListener::CWebsocketListener()
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
               socket.sendOneFrameText(processReceivedMessage(wholeMessage));
            }
            catch (const std::exception& e)
            {
               YADOMS_LOG(error) << "Error processing received message " << wholeMessage->c_str();
            }
         }
      }

      std::string CWebsocketListener::processReceivedMessage(const std::string receivedMessage)
      {
         const shared::CDataContainer frame(receivedMessage);
         const auto frameType = frame.get<std::string>("type");

         if (frameType == "isAlive")
         {
            shared::CDataContainer reply; //TODO mettre en static pour ne pas regénérer à chaque fois
            reply.set("type", "isAlive");
            return reply.serialize();
         }

         if (frameType == "acquisitionFilter")
         {
            const auto content = frame.get<boost::shared_ptr<shared::CDataContainer>>("content");
            //TODO
         }

         throw std::runtime_error("Invalid received message " + receivedMessage);
      }
   } //namespace oatppServer
} //namespace web
