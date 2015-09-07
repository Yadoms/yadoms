#include "stdafx.h"
#include "ContextAccessor.h"
#include "MessageQueueRemover.hpp"
#include <shared/DataContainer.h>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>

const size_t CContextAccessor::m_maxMessages(100);

CContextAccessor::CContextAccessor(shared::script::yScriptApi::IYScriptApi& context)
   :CThreadBase(createId()), m_context(context), m_id(createId()), m_readyBarrier(2)
{
   start();
   m_readyBarrier.wait();
}

CContextAccessor::~CContextAccessor()
{
}

std::string CContextAccessor::id() const
{
   return m_id;
}

std::string CContextAccessor::createId()
{
   std::stringstream ss;
   ss << "yPythonScript." << Poco::Process::id() << "." << boost::this_thread::get_id(); // Rule main thread id
   return ss.str();
}

void CContextAccessor::doWork()
{
   const std::string sendMessageQueueId(m_id + ".toScript");
   const std::string receiveMessageQueueId(m_id + ".toYadoms");
   const CMessageQueueRemover sendMessageQueueRemover(sendMessageQueueId);
   const CMessageQueueRemover receiveMessageQueueRemover(receiveMessageQueueId);
   try
   {
      YADOMS_LOG(debug) << "Open message queues";
      boost::interprocess::message_queue sendMessageQueue   (boost::interprocess::create_only, sendMessageQueueId.c_str()   , m_maxMessages, m_messageQueueMessageSize);
      boost::interprocess::message_queue receiveMessageQueue(boost::interprocess::create_only, receiveMessageQueueId.c_str(), m_maxMessages, m_messageQueueMessageSize);

      m_readyBarrier.wait();

      char message[m_messageQueueMessageSize];
      size_t messageSize;
      unsigned int messagePriority;
      while (true)
      {
         try
         {
            // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
            // polling and call boost::this_thread::interruption_point to exit properly
            bool messageWasReceived = receiveMessageQueue.timed_receive(message, m_messageQueueMessageSize, messageSize, messagePriority,
               boost::posix_time::ptime(boost::posix_time::second_clock::universal_time() + boost::posix_time::seconds(1)));
            boost::this_thread::interruption_point();

            if (messageWasReceived)
               processMessage(message, messageSize, sendMessageQueue);
         }
         catch (shared::exception::CInvalidParameter& ex)
         {
            YADOMS_LOG(error) << "Error receiving/processing queue message : " << ex.what();
         }
      }
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      YADOMS_LOG(error) << "Error creating/using message queues (" << m_id << ") in context accessor : " << ex.what();
   }
   catch (boost::thread_interrupted&)
   {
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Unknown error";
   }

   YADOMS_LOG(debug) << "Close message queues";
}

void CContextAccessor::sendAnswer(EAnswerIdentifier answerId, const shared::CDataContainer& answer, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer mainAnswerContainer;
   mainAnswerContainer.set("type", answerId);
   mainAnswerContainer.set("content", answer);

   const std::string serializedRequest(mainAnswerContainer.serialize());
   if (serializedRequest.size() > m_messageQueueMessageSize)
      throw std::overflow_error("sendRequest : answer is too big");
   messageQueue.send(serializedRequest.c_str(), serializedRequest.size(), 0);
}

void CContextAccessor::processMessage(const char* message, size_t messageSize, boost::interprocess::message_queue& messageQueue)
{
   if (messageSize < 1)
      throw shared::exception::CInvalidParameter("messageSize");

   // Unserialize message
   shared::CDataContainer mainRequestContainer(std::string(message, messageSize));
   if (!mainRequestContainer.exists("type") || !mainRequestContainer.exists("content"))
      throw shared::exception::CInvalidParameter("message"); 

   // Process message
   shared::CDataContainer request = mainRequestContainer.get<shared::CDataContainer>("content");
   std::string s = request.serialize();
   switch (mainRequestContainer.get<ERequestIdentifier>("type"))
   {
   case kReqReadKeyword:
   {
      shared::CDataContainer answer;
      answer.set("returnValue", m_context.readKeyword(request.get<int>("keywordId")));
      sendAnswer(kAnsReadKeyword, answer, messageQueue);
      break;
   }
   case kReqWaitForAcquisition:
   {
      shared::CDataContainer answer;
      answer.set("returnValue", m_context.waitForAcquisition(request.get<int>("keywordId"), request.get<std::string>("timeout")));
      sendAnswer(kAnsWaitForAcquisition, answer, messageQueue);
      break;
   }
   case kReqWaitForAcquisitions:
   {
      shared::CDataContainer answer;
      std::pair<int, std::string> returnValue = m_context.waitForAcquisitions(request.get<std::vector<int> >("keywordIdList"), request.get<std::string>("timeout"));
      answer.set("key", returnValue.first);
      answer.set("value", returnValue.second);
      sendAnswer(kAnsWaitForAcquisitions, answer, messageQueue);
      break;
   }
   case kReqWriteKeyword:
   {
      m_context.writeKeyword(request.get<int>("keywordId"), request.get<std::string>("newState"));
      break;
   }
   case kReqSendNotification:
   {
      m_context.sendNotification(request.get<int>("keywordId"), request.get<int>("recipientId"), request.get<std::string>("message"));
      break;
   }
   case kReqGetInfo:
   {
      shared::CDataContainer answer;
      answer.set("returnValue", m_context.getInfo(request.get<shared::script::yScriptApi::IYScriptApi::EInfoKeys>("key")));
      sendAnswer(kAnsGetInfo, answer, messageQueue);
      break;
   }
   case kReqRuleEnable:
   {
      m_context.ruleEnable(request.get<bool>("enable"));
      break;
   }

   default:
      throw shared::exception::CInvalidParameter("message");
   }
}