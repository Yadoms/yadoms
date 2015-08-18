#include "stdafx.h"
#include "ContextAccessor.h"
#include "MessageQueueRemover.hpp"
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

void CContextAccessor::processMessage(const char* message, size_t messageSize, boost::interprocess::message_queue& messageQueue)
{
   if (messageSize < 1)
      throw shared::exception::CInvalidParameter("messageSize");

   // Unserialize message id
   std::istringstream iss(std::string(message, messageSize), std::ios::binary);
   boost::archive::binary_iarchive ia(iss);

   ERequestIdentifier requestId;
   ia >> requestId;

   // Process message
   switch (requestId)
   {
   case kReqReadKeyword:
   {
      CReqReadKeyword request;
      ia >> request;

      CAnsReadKeyword answer;
      answer.m_returnValue = m_context.readKeyword(request.m_keywordId);
      sendAnswer(kAnsReadKeyword, answer, messageQueue);
      break;
   }
   case kReqWaitForAcquisition:
   {
      CReqWaitForAcquisition request;
      ia >> request;

      CAnsWaitForAcquisition answer;
      answer.m_returnValue = m_context.waitForAcquisition(request.m_keywordId, request.m_timeout);
      sendAnswer(kAnsWaitForAcquisition, answer, messageQueue);
      break;
   }
   case kReqWaitForAcquisitions:
   {
      CReqWaitForAcquisitions request;
      ia >> request;

      CAnsWaitForAcquisitions answer;
      answer.m_returnValue = m_context.waitForAcquisitions(request.m_keywordIdList, request.m_timeout);
      sendAnswer(kAnsWaitForAcquisitions, answer, messageQueue);
      break;
   }
   case kReqWriteKeyword:
   {
      CReqWriteKeyword request;
      ia >> request;
      
      m_context.writeKeyword(request.m_keywordId, request.m_newState);
      break;
   }
   case kReqSendNotification:
   {
      CReqSendNotification request;
      ia >> request;
      
      m_context.sendNotification(request.m_keywordId, request.m_recipientId, request.m_message);
      break;
   }
   case kReqGetInfo:
   {
      CReqGetInfo request;
      ia >> request;

      CAnsGetInfo answer;
      answer.m_returnValue = m_context.getInfo(request.m_key);
      sendAnswer(kAnsGetInfo, answer, messageQueue);
      break;
   }

   default:
      throw shared::exception::CInvalidParameter("message");
   }
}