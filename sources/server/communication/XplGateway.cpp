#include "stdafx.h"
#include "XplGateway.h"
#include <shared/Log.h>
#include <shared/xpl/XplConstants.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplService.h>
#include "command/DeviceCommand.h"
#include "rules/ICommandRule.h"

namespace communication {

   CXplGateway::CXplGateway(boost::shared_ptr<database::IDataProvider> dataProvider)
      :CThreadBase("XplGateway"), m_dataProvider(dataProvider)
   {
   }

   CXplGateway::~CXplGateway()
   {
   }

   void CXplGateway::sendCommandAsync(command::CDeviceCommand & message)
   {
      sendEvent(kXplSendMessage, message);
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
                  OnXplMessageReceived(xplMessage);
                  break;
               }

            case kXplSendMessage:
               {
                  command::CDeviceCommand command = popEvent<command::CDeviceCommand>();
                  OnSendDeviceCommand(command, xplService);
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


   //----------------------------------------------
   ///\brief Function handler when receiving XplMessage
   ///\param [in] The xpl message received
   //----------------------------------------------
   void CXplGateway::OnXplMessageReceived(shared::xpl::CXplMessage & xplMessage)
   {
      try
      {
         YADOMS_LOG(trace) << "Xpl Message received : " << xplMessage.toString();

         boost::shared_ptr<rules::IRule> rule = m_rulerFactory.identifyRule(xplMessage);
         if(rule.get() != NULL)
         {
            //create the device in database
            rules::DeviceIdentifier deviceAddress = rule->getDeviceAddressFromMessage(xplMessage);
            boost::shared_ptr<database::entities::CDevice> device = m_dataProvider->getDeviceRequester()->getDevice(deviceAddress, xplMessage.getMessageSchemaIdentifier().toString(), xplMessage.getSource().toString());

            //create message keywords in database
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
   }


   //----------------------------------------------
   ///\brief Function handler used to send a command to a device
   ///\param [in] The command to send
   //----------------------------------------------
   void CXplGateway::OnSendDeviceCommand(command::CDeviceCommand & message, shared::xpl::CXplService & xplService)
   {
      try
      {
         YADOMS_LOG(trace) << "Sending message : " << message.toString();


         //find device in database
         boost::shared_ptr<database::entities::CDevice> device = m_dataProvider->getDeviceRequester()->getDevice(message.getDeviceId());

         if(device.get() != NULL)
         {

            boost::shared_ptr< shared::xpl::CXplMessage > messageToSend = m_rulerFactory.createXplCommand(*device.get(), message);
            if(messageToSend)
            {
               messageToSend->setSource(xplService.getActor());
               xplService.sendMessage(*messageToSend.get());

               //send result
               if(message.getCallback().get() != NULL)
               {
                  command::CResult result = command::CResult::CreateSuccess();
                  message.getCallback()->sendResult(result);
               }
            }
            else
            {
               //send result
               std::string errorMessage = "Fail to create the Xpl message to send to the device";
               YADOMS_LOG(error) << errorMessage;

               if(message.getCallback().get() != NULL)
               {
                  command::CResult result = command::CResult::CreateError(errorMessage);
                  message.getCallback()->sendResult(result);
               }

            }
         }
         else
         {
            std::string errorMessage = (boost::format("Unknown device id = %1%") % message.getDeviceId()).str();
            YADOMS_LOG(error) << errorMessage;
            if(message.getCallback().get() != NULL)
            {
               command::CResult result = command::CResult::CreateError(errorMessage);               message.getCallback()->sendResult(result);
            }
         }

      }
      catch(std::exception &ex)
      {
         std::string errorMessage = (boost::format("CXplGateway fail to send message : %1%") % ex.what()).str();
         YADOMS_LOG(error) << errorMessage;

         if(message.getCallback().get() != NULL)
         {
            command::CResult result = command::CResult::CreateError(errorMessage);            message.getCallback()->sendResult(result);
         }
      }  
   }





} //namespace communication
