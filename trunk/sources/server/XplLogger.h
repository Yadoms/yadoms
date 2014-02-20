#pragma once
#include "database/IDataProvider.h"
#include <shared/ThreadBase.h>
#include <shared/Event/EventHandler.hpp>

class CXplLogger: public CThreadBase, public CEventHandler
{
public:
   CXplLogger(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CXplLogger();

private:
   enum
   {
      kXplMessageReceived = CEventHandler::kUserFirstId,
   };

private:
   // CThreadBase Implementation
   void doWork();
   // [END] CThreadBase Implementation

private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
};
