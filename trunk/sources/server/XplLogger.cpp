#include "stdafx.h"
#include "XplLogger.h"
#include <shared/Log.h>
#include <shared/xpl/XplConstants.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplService.h>


CXplLogger::CXplLogger(boost::shared_ptr<database::IDataProvider> dataProvider)
   :CThreadBase("XplLogger"), m_dataProvider(dataProvider)
{
}

CXplLogger::~CXplLogger()
{
}

void CXplLogger::doWork()
{
   try
   {
      YADOMS_LOG_CONFIGURE("XplLogger");
      YADOMS_LOG(debug) << "XplLogger is starting...";

      shared::xpl::CXplService xplService(shared::xpl::CXplConstants::getYadomsVendorId(), "logger", "1");

      //use this line to use be notified from shared::event::CEventHandler on an xplMessage
      xplService.messageReceived(this, kXplMessageReceived);

      while(1)
      {
         // Wait for an event
         switch(waitForEvents())
         {
         case kXplMessageReceived:
            {
               // Xpl message was received
               shared::xpl::CXplMessage xplMessage = popEvent<shared::xpl::CXplMessage>();
               //YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();
               try
               {
                  std::pair<std::string, std::string> bodyLine;
                  BOOST_FOREACH(bodyLine, xplMessage.getBody())
                  {
                     database::entities::CAcquisition acq;
                     acq.setSource(xplMessage.getSource().toString());
                     acq.setKeyword(bodyLine.first);
                     acq.setValue(bodyLine.second);
                     acq.setDate(boost::posix_time::second_clock::universal_time());
                     m_dataProvider->getAcquisitionRequester()->addAcquisition(acq);
                  }
               }
               catch(std::exception &ex)
               {
                  YADOMS_LOG(error) << "XplLogger fails to store message. " << ex.what();
               }
               catch(...)
               {
               }
               break;
            }

         case shared::event::kNoEvent:
            YADOMS_LOG(warning) << "CXplLogger::doWork, unknown event received";
            //do nothing
            break;

         default:
            {
               YADOMS_LOG(error) << "XplLogger : Unknown message id";

               // We need to consume this unknown event
               popEvent();

               break;
            }
         }
      };
   }
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "XplLogger is stopping..."  << std::endl;
   }
   catch(...)
   {
   }
}
