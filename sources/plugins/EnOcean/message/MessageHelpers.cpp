#include "stdafx.h"
#include "MessageHelpers.h"
#include "RadioErp1SendMessage.h"
#include "profiles/bitsetHelpers.hpp"
#include "ResponseReceivedMessage.h"

namespace message
{
   void CMessageHelpers::sendMessage(CRorgs::ERorgIds rorgId,
                                     boost::shared_ptr<IMessageHandler> messageHandler,
                                     const std::string& senderId,
                                     const std::string& targetId,
                                     const boost::dynamic_bitset<>& userData,
                                     const std::string& messageName)
   {
      CRadioErp1SendMessage command(rorgId,
                                    senderId,
                                    targetId,
                                    0);

      command.userData(bitset_to_bytes(userData));

      boost::shared_ptr<const CEsp3ReceivedPacket> answer;
      if (!messageHandler->send(command,
                                [](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                                {
                                   return esp3Packet->header().packetType() == RESPONSE;
                                },
                                [&](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                                {
                                   answer = esp3Packet;
                                }))
         throw std::runtime_error(
            (boost::format("Fail to send message to %1% : no answer to \"%2%\"") % targetId % messageName).str());

      const auto response = boost::make_shared<CResponseReceivedMessage>(answer);

      if (response->returnCode() != CResponseReceivedMessage::RET_OK)
         YADOMS_LOG(error) << "Fail to send message to " << targetId << " : \"" << messageName << "\" returns " <<
            response->returnCode();
   }
} // namespace message
