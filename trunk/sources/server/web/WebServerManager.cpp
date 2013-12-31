#include "stdafx.h"

#include "WebServerManager.h"
#include "IWebServer.h"
#include "tools/Log.h"
#include "rest/HardwareRestService.h"

CWebServerManager::CWebServerManager(boost::shared_ptr<IDataProvider> dataProvider, boost::shared_ptr<IWebServer> webServerInstance)
   : CThreadBase("WebServerManager"), m_webServerInstance(webServerInstance), m_dataProvider(dataProvider)
{
}

CWebServerManager::~CWebServerManager(void)
{
}

void CWebServerManager::doWork()
{
   YADOMS_LOG_CONFIGURE("WebServer");

   try
   {
      boost::shared_ptr<IRestHandler> restHanlder = m_webServerInstance->getRestHandler();
      if(restHanlder.get() != NULL)
      {
         YADOMS_LOG(info) << "WebServer supports REST api";
         restHanlder->configureRestService(boost::shared_ptr<IRestService>(new CHardwareRestService(m_dataProvider)));
      }

      YADOMS_LOG(info) << "WebServer is running...";
      while(1)
      {
         m_webServerInstance->start();
         boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(debug) << "WebServer is interrupted...";
   }
   YADOMS_LOG(info) << "WebServer is stopped.";

}

bool CWebServerManager::stop()
{
   if(m_webServerInstance.get() != NULL)
   {
      m_webServerInstance->stop();
   }
   return CThreadBase::stop();
}