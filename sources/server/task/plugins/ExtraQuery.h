#pragma once
#include "task/ITask.h"
#include <shared/event/EventHandler.hpp>
#include "pluginSystem/IInstance.h"

namespace task { namespace plugins {

   //------------------------------------------
   ///\brief   Extra query task
   //-----------------------------------------
   class CExtraQuery : public ITask
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      explicit CExtraQuery(boost::shared_ptr<pluginSystem::IInstance> pluginInstance, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> query);

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CExtraQuery();

   public:
      // ITask implementation
      virtual const std::string & getName();
      virtual void onSetTaskId(const std::string & taskId);
      virtual void doWork(TaskProgressFunc pFunctor);
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
