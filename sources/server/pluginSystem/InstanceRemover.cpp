#include "stdafx.h"
#include "InstanceRemover.h"

namespace pluginSystem
{
   CInstanceRemover::CInstanceRemover(boost::recursive_mutex& runningInstancesMutex,
                                      std::map<int, boost::shared_ptr<IInstance> >& runningInstances)
      : m_runningInstancesMutex(runningInstancesMutex),
        m_runningInstances(runningInstances)
   {
   }

   CInstanceRemover::~CInstanceRemover()
   {
   }

   void CInstanceRemover::onStopped(int instanceId)
   {
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_runningInstancesMutex);

         auto instance = m_runningInstances.find(instanceId);

         if (instance == m_runningInstances.end())
            return; // Already stopped ==> nothing more to do

         m_runningInstances.erase(instance);
      }

      {
         // Notify all handlers for this instance
         boost::lock_guard<boost::recursive_mutex> lock(m_instanceStopNotifiersMutex);
         auto itEventHandlerSetToNotify = m_instanceStopNotifiers.find(instanceId);
         if (itEventHandlerSetToNotify != m_instanceStopNotifiers.end())
            for (auto itHandler = itEventHandlerSetToNotify->second.begin(); itHandler != itEventHandlerSetToNotify->second.end(); ++itHandler)
               (*itHandler)->postEvent(shared::event::kUserFirstId);
      }
   }


   void CInstanceRemover::addWaiterOn(int instanceId,
                                      boost::shared_ptr<shared::event::CEventHandler> waiter)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_instanceStopNotifiersMutex);
      m_instanceStopNotifiers[instanceId].insert(waiter);
   }

   void CInstanceRemover::removeWaiterOn(int instanceId,
                                         boost::shared_ptr<shared::event::CEventHandler> waiter)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_instanceStopNotifiersMutex);
      m_instanceStopNotifiers[instanceId].erase(waiter);
   }
} // namespace pluginSystem


