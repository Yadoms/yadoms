#pragma once
#include "ISseConnectionHandler.h"
#include "notification/acquisition/Notification.hpp"
#include "notification/acquisition/Observer.hpp"
#include "notification/action/EventAction.hpp"
#include "shared/event/EventHandler.hpp"

namespace web
{
   namespace oatppServer
   {
      class CSseConnectionHandler final : public rest::service::ISseConnectionHandler
      {
      public:
         explicit CSseConnectionHandler(const std::unique_ptr<std::set<int>>& keywordIds);

         ~CSseConnectionHandler() override;

         CSseConnectionHandler(const CSseConnectionHandler&) = delete;
         CSseConnectionHandler(CSseConnectionHandler&&) = delete;
         CSseConnectionHandler& operator=(const CSseConnectionHandler&) = delete;
         CSseConnectionHandler& operator=(CSseConnectionHandler&&) = delete;

         std::pair<std::string, std::string> handle(const boost::posix_time::time_duration& timeout) const override;

      private:
         enum { kSseEventId = shared::event::kUserFirstId };

         boost::shared_ptr<shared::event::CEventHandler> m_streamingEventHandler = boost::make_shared<shared::event::CEventHandler>();
         const boost::shared_ptr<notification::acquisition::CObserver> m_acquisitionObserver = boost::make_shared<
            notification::acquisition::CObserver>(
            boost::make_shared<notification::action::CEventPtrAction<notification::acquisition::CNotification>>(
               m_streamingEventHandler,
               kSseEventId));
      };
   } //namespace oatppServer
} //namespace web
