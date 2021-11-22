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
   class CExtraQuery final : public shared::plugin::yPluginApi::IExtraQuery
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] data         The query data
      //-----------------------------------------------------
      explicit CExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> data);

      ~CExtraQuery() override = default;

      // IExtraQuery implementation
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> getData() const override;
      void sendSuccess(const boost::shared_ptr<shared::CDataContainer>& data) override;
      void sendError(const std::string& errorMessage) override;
      void reportProgress(float progression,
                          const std::string& message) override;
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
