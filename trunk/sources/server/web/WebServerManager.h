#pragma once

#include "IWebServer.h"
#include <shared/ThreadBase.h>

class CWebServerManager : public CThreadBase
{
public:
   CWebServerManager(boost::shared_ptr<IWebServer> webServerInstance);
   virtual ~CWebServerManager(void);
   void doWork();

   //override CThreadBase::stop to stop web server
   virtual bool stop();
private:
   boost::shared_ptr<IWebServer> m_webServerInstance;
};
