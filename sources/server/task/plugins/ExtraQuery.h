#pragma once
#include "task/ITask.h"
#include "pluginSystem/IInstance.h"

namespace task
{
   namespace plugins
   {
      //------------------------------------------
      ///\brief   Extra query task
      //-----------------------------------------
      class CExtraQuery final : public ITask
      {
      public:
         //------------------------------------------
         ///\brief   Constructor
         //------------------------------------------
         explicit CExtraQuery(const boost::shared_ptr<pluginSystem::IInstance>& pluginInstance,
                              const boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery>& query);
         ~CExtraQuery() override = default;

         // ITask implementation
         std::string getName() override;
         void onSetTaskId(const std::string& taskId) override;
         void doWork(TaskProgressFunc pFunctor) override;
         bool isCancellable() const override;
         // [END] ITask implementation

      private:
         //------------------------------------------
         ///\brief   The plugin instance
         //------------------------------------------
         boost::shared_ptr<pluginSystem::IInstance> m_pluginInstance;

         //------------------------------------------
         ///\brief   The extra query request
         //------------------------------------------
         boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> m_query;

         //------------------------------------------
         ///\brief   The task name
         //------------------------------------------
         std::string m_taskName;

         //------------------------------------------
         ///\brief   The task id
         //------------------------------------------
         std::string m_taskId;
      };
   } //namespace plugins
} //namespace task
