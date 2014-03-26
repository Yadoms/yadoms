#include "stdafx.h"
#include "XplGateway.h"
#include <shared/Log.h>
#include <shared/xpl/XplConstants.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplService.h>

namespace communication {

   CXplGateway::CXplGateway(boost::shared_ptr<database::IDataProvider> dataProvider)
      :CThreadBase("XplGateway"), m_dataProvider(dataProvider)
   {
   }

   CXplGateway::~CXplGateway()
   {
   }

   void CXplGateway::doWork()
   {
      try
      {
         YADOMS_LOG_CONFIGURE("XplGateway");
         YADOMS_LOG(debug) << "XplGateway is starting...";

         shared::xpl::CXplService xplService("gateway", "1", NULL, this, kXplMessageReceived);

         while(1)
         {
            // Wait for an event
            switch(waitForEvents())
            {
            case kXplMessageReceived:
               {
                  // Xpl message was received
                  shared::xpl::CXplMessage xplMessage = popEvent<shared::xpl::CXplMessage>();
                  YADOMS_LOG(trace) << "Xpl Message received : " << xplMessage.toString();

                  try
                  {

                     boost::shared_ptr<rules::IRule> rule = m_rulerFactory.identifyRule(xplMessage);
                     if(rule.get() != NULL)
                     {
                        rules::DeviceIdentifier deviceAddress = rule->GetDeviceAddressFromMessage(xplMessage);
                        boost::shared_ptr<database::entities::CDevice> device = m_dataProvider->getDeviceRequester()->getDevice(deviceAddress, xplMessage.getMessageSchemaIdentifier().toString(), xplMessage.getSource().toString());

                        database::entities::CMessage msgToInsert;
                        msgToInsert.setDate(boost::posix_time::second_clock::universal_time());
                        msgToInsert.setDeviceId(device->getId());

                        rules::MessageContent data = rule->ExtractMessageData(xplMessage);
                        std::vector<database::entities::CMessageContent> msgContentEntries;

                        rules::MessageContent::iterator i;
                        for(i = data.begin(); i!= data.end(); ++i)
                        {
                           database::entities::CMessageContent item;
                           item.setKey(i->first);
                           item.setValue(i->second);
                           msgContentEntries.push_back(item);
                        }

                        m_dataProvider->getMessageRequester()->insertMessage(msgToInsert, msgContentEntries);
                     }
                  }
                  catch(std::exception &ex)
                  {
                     YADOMS_LOG(error) << "CXplGateway fail to treat message : " << ex.what();
                  }


                  break;
               }

            case shared::event::kNoEvent:
               YADOMS_LOG(warning) << "CXplGateway::doWork, unknown event received";
               //do nothing
               break;

            default:
               {
                  YADOMS_LOG(error) << "CXplGateway : Unknown message id";

                  // We need to consume this unknown event
                  popEvent();

                  break;
               }
            }
         };
      }
      catch (boost::thread_interrupted&)
      {
         YADOMS_LOG(info) << "CXplGateway is stopping..."  << std::endl;
      }
      catch(...)
      {
      }
   }

} //namespace communication
