#pragma once
#include "database/IDataProvider.h"
#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>

class CXplLogger: public shared::CThreadBase, public shared::event::CEventHandler
{
public:
   CXplLogger(boost::shared_ptr<database::IDataProvider> dataProvider);
   virtual ~CXplLogger();

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
};
