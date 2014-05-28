#include "stdafx.h"
#include "PluginGateway.h"
#include <shared/Log.h>

namespace communication {


   enum { kStartEvent = shared::event::kUserFirstId };

   CPluginGateway::CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider)
      :CThreadBase("PluginGateway"), m_dataProvider(dataProvider)
   {
   }

   CPluginGateway::~CPluginGateway()
   {
   }

   void CPluginGateway::start()
   {
      CThreadBase::start();

      waitForstarted();
   }

   void CPluginGateway::waitForstarted()
   {
      switch (m_StartEventHandler.waitForEvents(boost::posix_time::seconds(10)))
      {
      case kStartEvent: // Gateway started
         return;
      case shared::event::kTimeout:
         BOOST_ASSERT_MSG(false, "Unable to start XPL gateway");
         throw shared::exception::CException("Unable to start XPL gateway");
         break;
      default:
         BOOST_ASSERT_MSG(false, "XPL gateway : unknown event");
         throw shared::exception::CException("XPL gateway : unknown event");
         break;
      }
   }

   void CPluginGateway::sendCommandAsync(command::CDeviceCommand & message)
   {
      sendEvent(kXplSendMessage, message);
   }


   void CPluginGateway::doWork()
   {
      try
      {
         YADOMS_LOG_CONFIGURE("PluginGateway");
         YADOMS_LOG(debug) << "PluginGateway is starting...";

         // Signal that gateway is fully started
         m_StartEventHandler.sendEvent(kStartEvent);

         while(1)
         {
            // Wait for an event
            switch(waitForEvents())
            {
            case shared::event::kNoEvent:
               YADOMS_LOG(warning) << "CPluginGateway::doWork, unknown event received";
               //do nothing
               break;

            default:
               {
                  YADOMS_LOG(error) << "CPluginGateway : Unknown message id";

                  // We need to consume this unknown event
                  popEvent();

                  break;
               }
            }
         };
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "CPluginGateway is stopping..."  << std::endl;
      }
      catch(...)
      {
      }
   }







} //namespace communication
