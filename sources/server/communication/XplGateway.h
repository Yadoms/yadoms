#pragma once
#include "database/IDataProvider.h"
#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "rules/RulerFactory.h"

namespace communication {

   class CXplGateway : public shared::CThreadBase, public shared::event::CEventHandler
   {
   public:
      CXplGateway(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CXplGateway();

   private:
      enum
      {
         kXplMessageReceived = shared::event::kUserFirstId,
      };

   private:
      // CThreadBase Implementation
      void doWork();
      // [END] CThreadBase Implementation

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      rules::CRulerFactory m_rulerFactory;
   };

} //namespace communication
