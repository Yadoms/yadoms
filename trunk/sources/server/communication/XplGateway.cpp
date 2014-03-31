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
                        //create the device in database
                        rules::DeviceIdentifier deviceAddress = rule->getDeviceAddressFromMessage(xplMessage);
                        boost::shared_ptr<database::entities::CDevice> device = m_dataProvider->getDeviceRequester()->getDevice(deviceAddress, xplMessage.getMessageSchemaIdentifier().toString(), xplMessage.getSource().toString());

                        //create keyword
                        std::vector< boost::shared_ptr<database::entities::CKeyword> > allKeywords = rule->identifyKeywords(xplMessage);
                        BOOST_FOREACH(boost::shared_ptr<database::entities::CKeyword> keyword, allKeywords)
                        {
                           keyword->DeviceId = device->Id();
                           m_dataProvider->getKeywordRequester()->addKeyword(keyword);
                        }

                        //create message to insert in database
                        database::entities::CMessage msgToInsert;
                        msgToInsert.Date = boost::posix_time::second_clock::universal_time();
                        msgToInsert.DeviceId = device->Id();

                        rules::MessageContent data = rule->extractMessageData(xplMessage);
                        std::vector<database::entities::CMessageContent> msgContentEntries;

                        rules::MessageContent::iterator i;
                        for(i = data.begin(); i!= data.end(); ++i)
                        {
                           database::entities::CMessageContent item;
                           item.Key = i->first;
                           item.Value = i->second;
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
