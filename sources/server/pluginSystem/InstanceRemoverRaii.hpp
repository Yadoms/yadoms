#pragma once
#include "InstanceRemover.h"

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	Instance remover RAII helper
   //--------------------------------------------------------------
   class CInstanceRemoverRaii//TODO virer
   {
   public:
      CInstanceRemoverRaii(boost::shared_ptr<CInstanceRemover> instanceRemover,
                           int id)
         : m_instanceRemover(instanceRemover),
           m_id(id),
           m_waitForStoppedInstanceHandler(boost::make_shared<shared::event::CEventHandler>())
      {
         m_instanceRemover->addWaiterOn(m_id, m_waitForStoppedInstanceHandler);
      }

      virtual ~CInstanceRemoverRaii()
      {
         m_instanceRemover->removeWaiterOn(m_id, m_waitForStoppedInstanceHandler);
      }

      boost::shared_ptr<shared::event::CEventHandler> eventHandler() const
      {
         return m_waitForStoppedInstanceHandler;
      }

   private:
      boost::shared_ptr<CInstanceRemover> m_instanceRemover;
      int m_id;
      boost::shared_ptr<shared::event::CEventHandler> m_waitForStoppedInstanceHandler;
   };
} // namespace pluginSystem


