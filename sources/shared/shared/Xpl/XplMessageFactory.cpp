#include "stdafx.h"
#include "XplMessageFactory.h"

#include "XplActor.h"
#include "XplMessageSchemaIdentifier.h"
#include "XplMessage.h"


CXplMessage CXplMessageFactory::createHeartbeatMessage(const CXplActor & source, const int heartbeatInterval, const std::string & remoteIpAddress, const int listeningPort)
{
   CXplActor actSource = CXplActor::createBroadcastActor();
   CXplMessageSchemaIdentifier hbeatMsgId = CXplMessageSchemaIdentifier::createHeartbeatMessageSchemaIdentifer();

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


