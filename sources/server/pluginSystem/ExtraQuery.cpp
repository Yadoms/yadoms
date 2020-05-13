#include "stdafx.h"
#include "ExtraQuery.h"
#include "communication/callback/CallbackRequest.h"
#include <shared/Log.h>
#include "i18n/ClientStrings.h"

namespace pluginSystem
{
   CExtraQuery::CExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> data)
      : m_data(data)
   {
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   enum
   {
      kFinished = shared::event::kUserFirstId, 
      kError
   };

   boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> CExtraQuery::getData() const
   {
      return m_data;
   }

   void CExtraQuery::sendSuccess(const shared::CDataContainerSharedPtr & data)
   {
      m_eventHandler.postEvent(kFinished, data);
   }

   void CExtraQuery::sendError(const std::string& errorMessage)
   {
      m_eventHandler.postEvent(kError, errorMessage);
   }

   void CExtraQuery::registerCallback(task::ITask::TaskProgressFunc callback)
   {
      m_progressNotifier = callback;
   }

   void CExtraQuery::waitForExtraQueryProcess()
   {
      bool running = true;

      while (running)
      {
         switch (m_eventHandler.waitForEvents())
         {
         case kFinished:
            m_progressNotifier(true, 100, i18n::CClientStrings::ExtraQuerySuccess, "", m_eventHandler.getEventData<shared::CDataContainerSharedPtr>());
            running = false;
            break;
         case kError:
            m_progressNotifier(false, 100, i18n::CClientStrings::ExtraQueryFail, m_eventHandler.getEventData<std::string>(), new_CDataContainerSharedPtr());
            running = false;
            break;
         }
      }
   }

   void CExtraQuery::reportProgress(const float progression, const std::string& message)
   {
      //use callback directly (if using m_eventHadler, there are bad side effects)
      m_progressNotifier(true, progression, message, "", new_CDataContainerSharedPtr());
   }

} // namespace pluginSystem	


