#pragma once

#include "IWebServer.h"
#include <shared/ThreadBase.h>

namespace web {

   //----------------------------------------
   ///\brief   Class which manage a WebServer.
   ///         The aim is to run a webserver into a thread
   //----------------------------------------
   class CWebServerManager : public shared::CThreadBase
   {
   public:
      //----------------------------------------
      ///\brief      Constructor
      ///\param [in] webServerInstance a web server instance to run into a thread
      //----------------------------------------
      CWebServerManager(boost::shared_ptr<IWebServer> webServerInstance);

      //----------------------------------------
      ///\brief      Destructor
      //----------------------------------------
      virtual ~CWebServerManager();

      // CThreadBase implmentation
      virtual bool stop();
   protected:
      void doWork();
      // [END] CThreadBase implmentation

   private:
      //----------------------------------------
      ///\brief      The web server instance
      //----------------------------------------
      boost::shared_ptr<IWebServer> m_webServerInstance;
   };

} //namespace web
