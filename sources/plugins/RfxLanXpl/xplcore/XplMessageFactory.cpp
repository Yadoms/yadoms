#include "stdafx.h"
#include "XplMessageFactory.h"

#include "XplActor.h"
#include "XplMessageSchemaIdentifier.h"
#include "XplMessage.h"

namespace xplcore
{
   CXplMessage CXplMessageFactory::createHeartbeatAppMessage(const CXplActor& source,
                                                             const int heartbeatInterval,
                                                             const std::string& remoteIpAddress,
                                                             const int listeningPort)
   {
      auto actSource = CXplActor::createBroadcastActor();
      auto hbeatMsgId = CXplMessageSchemaIdentifier::createHeartbeatAppMessageSchemaIdentifer();

      CXplMessage msg(CXplMessage::kXplStat, source, actSource, hbeatMsgId);
      msg.addToBody("interval", boost::lexical_cast<std::string>(heartbeatInterval));
      msg.addToBody("port", toString(listeningPort));
      msg.addToBody("remote-ip", remoteIpAddress);
      return msg;
   }

   CXplMessageFactory::CXplMessageFactory(void)
   {
   }


   CXplMessageFactory::~CXplMessageFactory(void)
   {
   }
} // namespace xplcore


