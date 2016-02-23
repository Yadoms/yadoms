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

void CContextAccessor::sendAnswer(const protobufMessage::Answer& answer, boost::interprocess::message_queue& messageQueue)
{
   if (!answer.IsInitialized())
      throw std::overflow_error("CContextAccessor::sendAnswer : answer is not fully initialized");

   if (answer.ByteSize() > m_messageQueueMessageSize)
      throw std::overflow_error("CContextAccessor::sendAnswer : answer is too big");

   if (!answer.SerializeToArray(m_mqBuffer, m_messageQueueMessageSize))
      throw std::overflow_error("CContextAccessor::sendAnswer : fail to serialize answer (too big ?)");

   messageQueue.send(m_mqBuffer, answer.GetCachedSize(), 0);
}

void CContextAccessor::processMessage(const void* message, size_t messageSize, boost::interprocess::message_queue& messageQueue)
{
   if (messageSize < 1)
      throw shared::exception::CInvalidParameter("messageSize");

   // Unserialize message
   protobufMessage::Request request;
   if (!request.ParseFromArray(message, messageSize))
      throw shared::exception::CInvalidParameter("message");

   // Process message
   if (request.has_getkeywordid())
      processGetKeywordId(request.getkeywordid(), messageQueue);
   else if (request.has_getrecipientid())
      processGetRecipientId(request.getrecipientid(), messageQueue);
   else if (request.has_readkeyword())
      processReadKeyword(request.readkeyword(), messageQueue);
   else if (request.has_waitfornextacquisition())
      processWaitForNextAcquisition(request.waitfornextacquisition(), messageQueue);
   else if (request.has_waitfornextacquisitions())
      processWaitForNextAcquisitions(request.waitfornextacquisitions(), messageQueue);
   else if (request.has_waitforevent())
      processWaitForEvent(request.waitforevent(), messageQueue);
   else if (request.has_writekeyword())
      processWriteKeyword(request.writekeyword(), messageQueue);
   else if (request.has_sendnotification())
      processSendNotification(request.sendnotification(), messageQueue);
   else if (request.has_getinfo())
      processGetInfo(request.getinfo(), messageQueue);
   else
      throw shared::exception::CInvalidParameter("message");
}

void CContextAccessor::processGetKeywordId(const protobufMessage::Request_GetKeywordId& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_GetKeywordId* answer = ans.mutable_getkeywordid();
   try
   {
      answer->set_id(m_scriptApi->getKeywordId(request.devicename(), request.keywordname()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processGetRecipientId(const protobufMessage::Request_GetRecipientId& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_GetRecipientId* answer = ans.mutable_getrecipientid();
   try
   {
      answer->set_id(m_scriptApi->getRecipientId(request.firstname(), request.lastname()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processReadKeyword(const protobufMessage::Request_ReadKeyword& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_ReadKeyword* answer = ans.mutable_readkeyword();
   try
   {
      answer->set_value(m_scriptApi->readKeyword(request.keywordid()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processWaitForNextAcquisition(const protobufMessage::Request_WaitForNextAcquisition& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_WaitForNextAcquisition* answer = ans.mutable_waitfornextacquisition();
   try
   {
      answer->set_acquisition(m_scriptApi->waitForNextAcquisition(request.keywordid(), request.has_timeout() ? request.timeout() : std::string()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processWaitForNextAcquisitions(const protobufMessage::Request_WaitForNextAcquisitions& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_WaitForNextAcquisitions* answer = ans.mutable_waitfornextacquisitions();
   try
   {
      std::vector<int> keywordIdList;
      for (google::protobuf::RepeatedField<google::protobuf::int32>::const_iterator it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
         keywordIdList.push_back(*it);
      std::pair<int, std::string> result = m_scriptApi->waitForNextAcquisitions(keywordIdList, request.has_timeout() ? request.timeout() : std::string());
      answer->set_keywordid(result.first);
      if (!result.second.empty())
         answer->set_acquisition(result.second);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processWaitForEvent(const protobufMessage::Request_WaitForEvent& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_WaitForEvent* answer = ans.mutable_waitforevent();
   try
   {
      std::vector<int> keywordIdList;
      for (google::protobuf::RepeatedField<google::protobuf::int32>::const_iterator it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
         keywordIdList.push_back(*it);
      shared::script::yScriptApi::CWaitForEventResult result = m_scriptApi->waitForEvent(keywordIdList, request.receivedatetimeevent(), request.has_timeout() ? request.timeout() : std::string());
      switch (result.getType())
      {
      case shared::script::yScriptApi::CWaitForEventResult::kTimeout:answer->set_type(protobufMessage::Answer_WaitForEvent_EventType::Answer_WaitForEvent_EventType_kTimeout); break;
      case shared::script::yScriptApi::CWaitForEventResult::kKeyword:answer->set_type(protobufMessage::Answer_WaitForEvent_EventType::Answer_WaitForEvent_EventType_kKeyword); break;
      case shared::script::yScriptApi::CWaitForEventResult::kDateTime:answer->set_type(protobufMessage::Answer_WaitForEvent_EventType::Answer_WaitForEvent_EventType_kDateTime); break;
      default:
         throw shared::exception::CInvalidParameter("CWaitForEventResult::type");
      }
      answer->set_keywordid(result.getKeywordId());
      if (!result.getValue().empty())
         answer->set_acquisition(result.getValue());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processWriteKeyword(const protobufMessage::Request_WriteKeyword& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   try
   {
      m_scriptApi->writeKeyword(request.keywordid(), request.newstate());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processSendNotification(const protobufMessage::Request_SendNotification& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   try
   {
      m_scriptApi->sendNotification(request.keywordid(), request.recipientid(), request.message());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}

void CContextAccessor::processGetInfo(const protobufMessage::Request_GetInfo& request, boost::interprocess::message_queue& messageQueue)
{
   protobufMessage::Answer ans;
   protobufMessage::Answer_GetInfo* answer = ans.mutable_getinfo();
   try
   {
      shared::script::yScriptApi::IYScriptApi::EInfoKeys key;
      switch (request.key())
      {
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kSunrise: key = shared::script::yScriptApi::IYScriptApi::kSunrise; break;
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kSunset: key = shared::script::yScriptApi::IYScriptApi::kSunset; break;
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kLatitude: key = shared::script::yScriptApi::IYScriptApi::kLatitude; break;
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kLongitude: key = shared::script::yScriptApi::IYScriptApi::kLongitude; break;
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kAltitude: key = shared::script::yScriptApi::IYScriptApi::kAltitude; break;
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kYadomsServerOS: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerOS; break;
      case protobufMessage::Request_GetInfo_Key::Request_GetInfo_Key_kYadomsServerVersion: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerVersion; break;
      default:
         throw shared::exception::CInvalidParameter("answer.waitforeventrequestanswer.type");
      }

      answer->set_value(m_scriptApi->getInfo(key));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }
   sendAnswer(ans, messageQueue);
}