#pragma once
#include "database/IDataProvider.h"
#include <shared/Xpl/XplMessage.h>
#include <shared/Xpl/XplService.h>

class CXplLogger
{
public:
   CXplLogger(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CXplLogger();

   void start();
   void stop();
private:
   void onMessageReceived(CXplMessage & message);

private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   boost::shared_ptr<CXplService> m_xplService;
};