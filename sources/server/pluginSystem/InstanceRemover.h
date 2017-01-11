#pragma once
#include "IInstanceStoppedListener.h"
#include "IInstance.h"
#include <shared/event/EventHandler.hpp>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Instance remover when stopped
   //--------------------------------------------------------------
   class CInstanceRemover : public IInstanceStoppedListener //TODO virer
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] runningInstancesMutex  Mutex on running instance list
      /// \param[in] runningInstances  Running instance list
      //--------------------------------------------------------------
      CInstanceRemover(boost::recursive_mutex& runningInstancesMutex,
                       std::map<int, boost::shared_ptr<IInstance> >& runningInstances);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstanceRemover();

      // IInstanceStoppedListener Implementation
      void onStopped(int instanceId) override;
      // [END] IInstanceStoppedListener Implementation

      void addWaiterOn(int instanceId, boost::shared_ptr<shared::event::CEventHandler> waiter);
      void removeWaiterOn(int instanceId, boost::shared_ptr<shared::event::CEventHandler> waiter);

   private:
      boost::recursive_mutex& m_runningInstancesMutex;
      std::map<int, boost::shared_ptr<IInstance> >& m_runningInstances;

      boost::recursive_mutex m_instanceStopNotifiersMutex;
      std::map<int, std::set<boost::shared_ptr<shared::event::CEventHandler> > > m_instanceStopNotifiers;
   };
} // namespace pluginSystem

