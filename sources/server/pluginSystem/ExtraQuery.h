#pragma once
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/plugin/yPluginApi/IExtraQueryData.h>
#include "task/ITask.h"
#include <shared/event/EventHandler.hpp>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IExtraQuery implementation
   //-----------------------------------------------------
   class CExtraQuery : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] query        The extra query
      ///\param[in] data         The query data
      ///\param [in]  callback   The callback to call when request returns
      //-----------------------------------------------------
      explicit CExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> data);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CExtraQuery();

      // IExtraQuery implementation
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> getData() const override;
      void sendSuccess(const shared::CDataContainer& data) override;
      void sendError(const std::string& errorMessage) override;
      void reportProgress(const float progression, const std::string& message) override;
      // [END] IExtraQuery implementation

      //-----------------------------------------------------
      ///\brief                  Wait for extra query process. It wait for ends of extra query, and use callback (@see registerCallback) to update progression
      //-----------------------------------------------------
      void waitForExtraQueryProcess();

      //-----------------------------------------------------
      ///\brief                  Register the progression notification callback
      ///\param [in]  callback   The callback to notify for progression update
      //-----------------------------------------------------
      void registerCallback(task::ITask::TaskProgressFunc callback);
      
   private:
      //-----------------------------------------------------
      ///\brief Internal data
      //-----------------------------------------------------
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> m_data;

      task::ITask::TaskProgressFunc m_progressNotifier;
      //-----------------------------------------------------
      ///\brief Internal event handler
      //-----------------------------------------------------
      shared::event::CEventHandler m_eventHandler;
   };
} // namespace pluginSystem	


