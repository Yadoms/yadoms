#include "stdafx.h"
#include "ContextAccessor.h"
#include "MessageQueueRemover.hpp"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>

const size_t CContextAccessor::m_maxMessages(100);

CContextAccessor::CContextAccessor(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi)
   :CThreadBase(createId()), m_scriptApi(yScriptApi), m_id(createId()), m_readyBarrier(2)
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
   ss << "yPythonScript." << boost::uuids::random_generator()(); // Rule main thread id
   return ss.str();
}

void CContextAccessor::doWork()
{
   // Verify that the version of the library that we linked against is
   // compatible with the version of the headers we compiled against.
   GOOGLE_PROTOBUF_VERIFY_VERSION;

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

      unsigned char message[m_messageQueueMessageSize];
      size_t messageSize;
      unsigned int messagePriority;
      while (true)
      {
         try
         {
            // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
            // polling and call boost::this_thread::interruption_point to exit properly
            // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
            bool messageWasReceived = receiveMessageQueue.timed_receive(message, m_messageQueueMessageSize, messageSize, messagePriority,
               boost::posix_time::ptime(boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1)));
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

   // Delete all global objects allocated by libprotobuf.
   google::protobuf::ShutdownProtobufLibrary();
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

void CContextAccessor::processMessage(const void* message, size_t messageSize, boost::interprocess::message_queue& messageQueue)
{
   if (messageSize < 1)
      throw shared::exception::CInvalidParameter("messageSize");

   // Unserialize message
   wrapperMessages::ToYadoms request;
   if (!request.ParseFromArray(message, messageSize))
      throw shared::exception::CInvalidParameter("message");

   // Process message
   if (request.has_keywordidrequest())
      processGetKeywordId(request.keywordidrequest(), messageQueue);

   //TODO nettoyage
   //shared::CDataContainer request = mainRequestContainer.get<shared::CDataContainer>("content");
   //switch (mainRequestContainer.get<ERequestIdentifier>("type"))
   //{
   //case kReqGetKeywordId            : processGetKeywordId(request, messageQueue); break;
   //case kReqGetRecipientId          : processGetRecipientId(request, messageQueue); break;
   //case kReqReadKeyword             : processReadKeyword(request, messageQueue); break;
   //case kReqWaitForNextAcquisition  : processWaitForNextAcquisition(request, messageQueue); break;
   //case kReqWaitForNextAcquisitions : processWaitForNextAcquisitions(request, messageQueue); break;
   //case kReqWaitForEvent            : processWaitForEvent(request, messageQueue); break;
   //case kReqWriteKeyword            : processWriteKeyword(request, messageQueue); break;
   //case kReqSendNotification        : processSendNotification(request, messageQueue); break;
   //case kReqGetInfo                 : processGetInfo(request, messageQueue); break;
   //default:
   //   throw shared::exception::CInvalidParameter("message");
   //}
}

void CContextAccessor::processGetKeywordId(const wrapperMessages::ToYadoms_KeywordIdRequest& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      answer.set("returnValue", m_scriptApi->getKeywordId(request.devicename(), request.keywordname()));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsGetKeywordId, answer, messageQueue);
}

void CContextAccessor::processGetKeywordId(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      answer.set("returnValue", m_scriptApi->getKeywordId(request.get<std::string>("device"), request.get<std::string>("keyword")));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsGetKeywordId, answer, messageQueue);
}

void CContextAccessor::processGetRecipientId(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      answer.set("returnValue", m_scriptApi->getRecipientId(request.get<std::string>("firstName"), request.get<std::string>("lastName")));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsGetRecipientId, answer, messageQueue);
}

void CContextAccessor::processReadKeyword(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      answer.set("returnValue", m_scriptApi->readKeyword(request.get<int>("keywordId")));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsReadKeyword, answer, messageQueue);
}

void CContextAccessor::processWaitForNextAcquisition(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      answer.set("returnValue", m_scriptApi->waitForNextAcquisition(request.get<int>("keywordId"), request.get<std::string>("timeout")));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsWaitForAcquisition, answer, messageQueue);
}

void CContextAccessor::processWaitForNextAcquisitions(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      std::pair<int, std::string> returnValue = m_scriptApi->waitForNextAcquisitions(request.get<std::vector<int> >("keywordIdList"), request.get<std::string>("timeout"));
      answer.set("key", returnValue.first);
      answer.set("value", returnValue.second);
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsWaitForAcquisitions, answer, messageQueue);
}

void CContextAccessor::processWaitForEvent(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      shared::script::yScriptApi::CWaitForEventResult returnValue = m_scriptApi->waitForEvent(request.get<std::vector<int> >("keywordIdList"), request.get<bool>("receiveDateTimeEvent"), request.get<std::string>("timeout"));
      
      answer.set("type", returnValue.getType());
      answer.set("keywordId", returnValue.getKeywordId());
      answer.set("value", returnValue.getValue());
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsWaitForEvent, answer, messageQueue);
}

void CContextAccessor::processWriteKeyword(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      m_scriptApi->writeKeyword(request.get<int>("keywordId"), request.get<std::string>("newState"));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsWriteKeyword, answer, messageQueue);
}

void CContextAccessor::processSendNotification(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      m_scriptApi->sendNotification(request.get<int>("keywordId"), request.get<int>("recipientId"), request.get<std::string>("message"));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsSendNotification, answer, messageQueue);
}

void CContextAccessor::processGetInfo(const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue)
{
   shared::CDataContainer answer;
   try
   {
      answer.set("returnValue", m_scriptApi->getInfo(request.get<shared::script::yScriptApi::IYScriptApi::EInfoKeys>("key")));
   }
   catch (std::exception& ex)
   {
      answer.set("error", ex.what());
   }
   sendAnswer(kAnsGetInfo, answer, messageQueue);
}