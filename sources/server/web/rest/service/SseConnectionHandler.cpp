#include "stdafx.h"
#include "SseConnectionHandler.h"

using namespace web::oatppServer;


CSseConnectionHandler::CSseConnectionHandler(boost::shared_ptr<shared::event::CEventHandler> streamingEventHandler,
                                             int streamingOnNewEventId)
   : m_streamingEventHandler(std::move(streamingEventHandler)),
     m_streamingOnNewEventId(streamingOnNewEventId)
{
}

CSseConnectionHandler::~CSseConnectionHandler()
{
   //TODO unscubscibre
}

std::string CSseConnectionHandler::handle() const
{
   if (m_streamingEventHandler->waitForEvents() == m_streamingOnNewEventId)
      return "event: hello !\n"
         "data: <some data>\n\n";
   return {};
}
