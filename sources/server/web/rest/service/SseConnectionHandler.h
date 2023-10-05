#pragma once
#include "ISseConnectionHandler.h"
#include "shared/event/EventHandler.hpp"

namespace web
{
   namespace oatppServer
   {
      class CSseConnectionHandler final : public rest::service::ISseConnectionHandler
      {
      public:
         explicit CSseConnectionHandler(boost::shared_ptr<shared::event::CEventHandler> streamingEventHandler,
                                        int streamingOnNewEventId);

         ~CSseConnectionHandler() override;

         std::string handle() const override;

      private:
         const boost::shared_ptr<shared::event::CEventHandler> m_streamingEventHandler = nullptr;
         const int m_streamingOnNewEventId;
      };
   } //namespace oatppServer
} //namespace web
