#include "stdafx.h"
#include "MessageHelpers.h"

#include <utility>
#include "radioErp1/SendMessage.h"
#include "profiles/bitsetHelpers.hpp"
#include "response/ReceivedMessage.h"

namespace message
{
   void CMessageHelpers::sendMessage(CRorgs::ERorgIds rorgId,
                                     const boost::shared_ptr<IMessageHandler>& messageHandler,
                                     const std::string& senderId,
                                     const std::string& targetId,
                                     const boost::dynamic_bitset<>& userData,
                                     const std::string& messageName)
   {
      radioErp1::CSendMessage command(rorgId,
                                      senderId,
                                      targetId,
                                      0);

      command.userData(bitset_to_bytes(userData));

      sendMessage(command,
                  messageHandler,
                  messageName);
   }

   void CMessageHelpers::sendMessage(radioErp1::CSendMessage& message,
                                     const boost::shared_ptr<IMessageHandler>& messageHandler,
                                     const std::string& messageName)
   {
      boost::shared_ptr<const CEsp3ReceivedPacket> answer;

      if (!messageHandler->send(message,
                                [](const boost::shared_ptr<const CEsp3ReceivedPacket>& esp3Packet)
                                {
                                   return esp3Packet->header().packetType() == RESPONSE;
                                },
                                [&](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                                {
                                   answer = std::move(esp3Packet);
                                }))
         throw std::runtime_error(std::string("Fail to send message to ")
                                  + message.destinationId()
                                  + " : no answer to \""
                                  + messageName
                                  + "\"");

      if (const auto response = boost::make_shared<response::CReceivedMessage>(answer);
          response->returnCode() != response::CReceivedMessage::RET_OK)
         YADOMS_LOG(error) << "Fail to send message to " << message.destinationId() << " : \"" << messageName << "\" returns " << response->returnCode();
   }
} // namespace message
