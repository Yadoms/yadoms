#include "stdafx.h"
#include "Ack.h"
#include "MessagesDefinition.h"

namespace nitramMessages
{
   CAck::CAck()
   {}

   CAck::~CAck()
   {}

   void CAck::decode(boost::shared_ptr<yApi::IYPluginApi> api,
                     const shared::communication::CByteBuffer& buffer)
   {
      // Nothing to do
   }


   boost::shared_ptr<shared::communication::CByteBuffer> CAck::encode() const
   {
      shared::communication::CByteBuffer message(1);

      // The ID of the message
      message[0] = nitramMessages::ENitramMessageID::Ack;
      return boost::make_shared<shared::communication::CByteBuffer>(message);
   }

   void CAck::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Nothing to historize
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CAck::keywords() const
   {
      return NoKeywords;
   }

   bool CAck::onlyAck() const
   {
      //TODO : Faire un throw !
      return true;
   }

} // namespace nitramMessages