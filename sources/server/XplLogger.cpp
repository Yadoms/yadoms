#include "stdafx.h"
#include "XplLogger.h"
#include <shared/Log.h>
#include <shared/Xpl/XplConstants.h>
#include <shared/Xpl/XplMessage.h>
#include <shared/Xpl/XplService.h>


CXplLogger::CXplLogger(boost::shared_ptr<server::database::IDataProvider> dataProvider)
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

      CXplService xplService(CXplConstants::getYadomsVendorId(), "logger", "1");

      //use this line to use be notified from CEventHandler on an xplMessage
      xplService.messageReceived(this, kXplMessageReceived);

      while(1)
      {
         // Wait for an event, with timeout
         switch(waitForEvents(boost::posix_time::milliseconds(500)))
         {
         case kXplMessageReceived:
            {
               // Xpl message was received
               CXplMessage xplMessage = popEvent<CXplMessage>();
               //YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();
               try
               {
                  std::pair<std::string, std::string> bodyLine;
                  BOOST_FOREACH(bodyLine, xplMessage.getBody())
                  {
                     server::database::entities::CAcquisition acq;
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

         case kTimeout:
         case kNoEvent:
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
