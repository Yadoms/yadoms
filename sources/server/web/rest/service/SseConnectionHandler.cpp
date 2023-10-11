#include "stdafx.h"
#include "SseConnectionHandler.h"

#include "notification/Helpers.hpp"
#include "notification/acquisition/Notification.hpp"

using namespace web::oatppServer;


CSseConnectionHandler::CSseConnectionHandler(const std::unique_ptr<std::set<int>>& keywordIds)
{
   if (keywordIds->empty())
      return;

   m_acquisitionObserver->resetKeywordIdFilter(*keywordIds);
   notification::CHelpers::subscribeCustomObserver(m_acquisitionObserver);
}

CSseConnectionHandler::~CSseConnectionHandler()
{
   notification::CHelpers::unsubscribeObserver(m_acquisitionObserver);
}

std::pair<std::string, std::string> CSseConnectionHandler::handle(const boost::posix_time::time_duration& timeout) const
{
   if (m_streamingEventHandler->waitForEvents(timeout) == kSseEventId)
   {
      const auto newAcquisition = m_streamingEventHandler->getEventData<boost::shared_ptr<notification::acquisition::CNotification>>()->
                                                           getAcquisition();
      return {
         "new acquisition received",
         "keyword=" + std::to_string(newAcquisition->KeywordId()) + ", value=" + newAcquisition->Value()
      };
   }
   return {};
}
