#include "stdafx.h"
#include "ExtraQuery.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CExtraQuery::CExtraQuery(const shared::plugin::yPluginApi::IExtraQueryData& data)
      : m_data(data)
   {
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   enum
   {
      kProgress = shared::event::kUserFirstId,
      kFinished,
      kError
   };


   const shared::plugin::yPluginApi::IExtraQueryData& CExtraQuery::getData() const
   {
      return m_data;
   }

   void CExtraQuery::sendSuccess(const shared::CDataContainer& data)
   {
      m_eventHandler.postEvent(kFinished, data);
   }

   void CExtraQuery::sendError(const std::string& errorMessage)
   {
      m_eventHandler.postEvent(kError, errorMessage);
   }

   void CExtraQuery::waitForExtraQueryProcess(task::ITask::TaskProgressFunc progressNotifier)
   {
      bool running = true;

      while (running)
      {
         switch (m_eventHandler.waitForEvents())
         {
         case kProgress:
         {
            auto dc = m_eventHandler.getEventData<shared::CDataContainer>();
            progressNotifier(true, dc.get<float>("progression"), dc.get("message"), "", shared::CDataContainer());
            break;
         }
         case kFinished:
            progressNotifier(false, 100, "", "", m_eventHandler.getEventData<shared::CDataContainer>());
            running = false;
            break;
         case kError:
            progressNotifier(false, 100, m_eventHandler.getEventData<std::string>(), "", shared::CDataContainer());
            running = false;
            break;
         }
      }
   }

   void CExtraQuery::reportProgress(const float progression, const std::string& message)
   {
      shared::CDataContainer dc;
      dc.set("progression", progression);
      dc.set("message", message);
      m_eventHandler.postEvent(kProgress, dc);
   }

} // namespace pluginSystem	


