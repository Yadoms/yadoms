#include "stdafx.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "task/ITask.h"
#include "ExtraQuery.h"
#include "pluginSystem/ExtraQuery.h"

namespace task { namespace plugins {

   std::string CExtraQuery::m_taskName = "plugins.extraQuery";

   CExtraQuery::CExtraQuery(boost::shared_ptr<pluginSystem::IInstance> pluginInstance, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> query)
      :m_pluginInstance(pluginInstance), m_query(query)
   {
      if(!m_pluginInstance)
         throw shared::exception::CInvalidParameter("pluginInstance");
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   const std::string & CExtraQuery::getName()
   {
      return m_taskName;
   }

   void CExtraQuery::onSetTaskId(const std::string & taskId)
   {
      m_taskId = taskId;
   }

   void CExtraQuery::doWork(TaskProgressFunc pFunctor)
   {
      boost::shared_ptr<pluginSystem::CExtraQuery> queryReal = boost::dynamic_pointer_cast<pluginSystem::CExtraQuery>(m_query);
      queryReal->registerCallback(pFunctor);
         
      m_pluginInstance->postExtraQuery(m_query, m_taskId);

      queryReal->waitForExtraQueryProcess();
   }

} //namespace plugins
} //namespace task
