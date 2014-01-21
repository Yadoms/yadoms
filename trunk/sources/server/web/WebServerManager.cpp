#include "stdafx.h"

#include "WebServerManager.h"
#include "IWebServer.h"
#include "tools/Log.h"


CWebServerManager::CWebServerManager(boost::shared_ptr<IWebServer> webServerInstance)
   : CThreadBase("WebServerManager"), m_webServerInstance(webServerInstance)
{
}

CWebServerManager::~CWebServerManager(void)
{
   stop();
}

void CWebServerManager::doWork()
{
   YADOMS_LOG_CONFIGURE("WebServer");

   try
   {
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
   catch(...)
   {
      YADOMS_LOG(debug) << "WebServer is interrupted from un unusual way...";
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