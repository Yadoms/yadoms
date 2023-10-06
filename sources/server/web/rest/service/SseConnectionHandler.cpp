#include "stdafx.h"
#include "SseConnectionHandler.h"

#include "notification/acquisition/Notification.hpp"

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
   {
      const auto newAcquisition = m_streamingEventHandler->getEventData<boost::shared_ptr<notification::acquisition::CNotification>>()->
                                                           getAcquisition();
      return "event: new acquisition received\n"
         "data: keyword=" + std::to_string(newAcquisition->KeywordId()) + ", value=" + newAcquisition->Value() + "\n\n";
   }
   return {};
}
