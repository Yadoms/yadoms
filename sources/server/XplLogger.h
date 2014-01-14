#pragma once
#include "database/IDataProvider.h"
#include "tools/Xpl/XplService.h"
#include "tools/Xpl/XplMessage.h"

class CXplLogger
{
public:
   CXplLogger(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CXplLogger();

private:
   void onMessageReceived(CXplMessage & message);

private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   boost::shared_ptr<CXplService> m_xplService;
};