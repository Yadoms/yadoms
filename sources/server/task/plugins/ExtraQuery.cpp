#include "stdafx.h"
#include <shared/exception/InvalidParameter.hpp>
#include "task/ITask.h"
#include "ExtraQuery.h"
#include "pluginSystem/ExtraQuery.h"

namespace task
{
   namespace plugins
   {
      CExtraQuery::CExtraQuery(boost::shared_ptr<pluginSystem::IInstance> pluginInstance, boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> query)
         : m_pluginInstance(pluginInstance), m_query(query), m_taskName("plugins/" + pluginInstance->aboutPlugin()->getType() + ":extraQueries." + query->getData()->query() + ".name")
      {
         if (!m_pluginInstance)
            throw shared::exception::CInvalidParameter("pluginInstance");
      }

      CExtraQuery::~CExtraQuery()
      {
      }

      const std::string& CExtraQuery::getName() const
      {
         return m_taskName;
      }

      void CExtraQuery::onSetTaskId(const std::string& taskId)
      {
         m_taskId = taskId;
      }

      void CExtraQuery::doWork(TaskProgressFunc pFunctor)
      {
         auto queryReal = boost::dynamic_pointer_cast<pluginSystem::CExtraQuery>(m_query);
         queryReal->registerCallback(pFunctor);

         m_pluginInstance->postExtraQuery(m_query, m_taskId);

         queryReal->waitForExtraQueryProcess();
      }
   } //namespace plugins
} //namespace task


