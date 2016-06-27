#include "stdafx.h"
#include "ContextAccessor.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>

const size_t CContextAccessor::m_maxMessages(100);

CContextAccessor::CContextAccessor(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi)
   :CThreadBase(createId()), m_scriptApi(yScriptApi), m_id(createId()), m_readyBarrier(2)
{
   memset(m_mqBuffer, 0, sizeof(m_mqBuffer));
   CThreadBase::start();
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
   const shared::communication::CMessageQueueRemover sendMessageQueueRemover(sendMessageQueueId);
   const shared::communication::CMessageQueueRemover receiveMessageQueueRemover(receiveMessageQueueId);
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
            auto messageWasReceived = receiveMessageQueue.timed_receive(message, m_messageQueueMessageSize, messageSize, messagePriority,
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

void CContextAccessor::sendAnswer(const pbAnswer::msg& answer, boost::interprocess::message_queue& messageQueue)
{
   if (!answer.IsInitialized())
      throw std::overflow_error("CContextAccessor::sendAnswer : answer is not fully initialized");

   if (answer.ByteSize() > static_cast<int>(m_messageQueueMessageSize))
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
   pbRequest::msg request;
   if (!request.ParseFromArray(message, messageSize))
      throw shared::exception::CInvalidParameter("message : fail to parse received data into protobuf format");

   // Process message
   switch(request.OneOf_case())
   {
   case pbRequest::msg::kGetKeywordId: processGetKeywordId(request.getkeywordid(), messageQueue); break;
   case pbRequest::msg::kGetRecipientId: processGetRecipientId(request.getrecipientid(), messageQueue); break;
   case pbRequest::msg::kReadKeyword: processReadKeyword(request.readkeyword(), messageQueue); break;
   case pbRequest::msg::kWaitForNextAcquisition: processWaitForNextAcquisition(request.waitfornextacquisition(), messageQueue); break;
   case pbRequest::msg::kWaitForNextAcquisitions: processWaitForNextAcquisitions(request.waitfornextacquisitions(), messageQueue); break;
   case pbRequest::msg::kWaitForEvent: processWaitForEvent(request.waitforevent(), messageQueue); break;
   case pbRequest::msg::kGetKeywordsByCapacity: processGetKeywordsByCapacity(request.getkeywordsbycapacity(), messageQueue); break;
   case pbRequest::msg::kWriteKeyword: processWriteKeyword(request.writekeyword(), messageQueue); break;
   case pbRequest::msg::kSendNotification: processSendNotification(request.sendnotification(), messageQueue); break;
   case pbRequest::msg::kGetInfo: processGetInfo(request.getinfo(), messageQueue); break;
   case pbRequest::msg::kGetKeywordName: processGetKeywordName(request.getkeywordname(), messageQueue); break;
   case pbRequest::msg::kGetKeywordDeviceName: processGetKeywordDeviceName(request.getkeyworddevicename(), messageQueue); break;
   default:
      throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % request.OneOf_case()).str());
   }
}

void CContextAccessor::processGetKeywordId(const pbRequest::GetKeywordId& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getkeywordid();
   try
   {
      answer->set_id(m_scriptApi->getKeywordId(request.devicename(), request.keywordname()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch(std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to GetKeywordId request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processGetRecipientId(const pbRequest::GetRecipientId& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::GetRecipientId* answer = ans.mutable_getrecipientid();
   try
   {
      answer->set_id(m_scriptApi->getRecipientId(request.firstname(), request.lastname()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to GetRecipientId request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processReadKeyword(const pbRequest::ReadKeyword& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::ReadKeyword* answer = ans.mutable_readkeyword();
   try
   {
      answer->set_value(m_scriptApi->readKeyword(request.keywordid()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to ReadKeyword request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processWaitForNextAcquisition(const pbRequest::WaitForNextAcquisition& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::WaitForNextAcquisition* answer = ans.mutable_waitfornextacquisition();
   try
   {
      answer->set_acquisition(m_scriptApi->waitForNextAcquisition(request.keywordid(), request.has_timeout() ? request.timeout() : std::string()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to WaitForNextAcquisition request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processWaitForNextAcquisitions(const pbRequest::WaitForNextAcquisitions& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::WaitForNextAcquisitions* answer = ans.mutable_waitfornextacquisitions();
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

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to WaitForNextAcquisitions request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processWaitForEvent(const pbRequest::WaitForEvent& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::WaitForEvent* answer = ans.mutable_waitforevent();
   try
   {
      std::vector<int> keywordIdList;
      for (google::protobuf::RepeatedField<google::protobuf::int32>::const_iterator it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
         keywordIdList.push_back(*it);

      shared::script::yScriptApi::CWaitForEventResult result = m_scriptApi->waitForEvent(keywordIdList, request.receivedatetimeevent(), request.has_timeout() ? request.timeout() : std::string());
      switch (result.getType())
      {
      case shared::script::yScriptApi::CWaitForEventResult::kTimeout:answer->set_type(pbAnswer::WaitForEvent_EventType_kTimeout); break;
      case shared::script::yScriptApi::CWaitForEventResult::kKeyword:answer->set_type(pbAnswer::WaitForEvent_EventType_kKeyword); break;
      case shared::script::yScriptApi::CWaitForEventResult::kDateTime:answer->set_type(pbAnswer::WaitForEvent_EventType_kDateTime); break;
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

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to WaitForEvent request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processGetKeywordsByCapacity(const pbRequest::GetKeywordsByCapacity& request, boost::interprocess::message_queue& messageQueue)
{

   pbAnswer::msg ans;
   pbAnswer::GetKeywordsByCapacity* answer = ans.mutable_getkeywordsbycapacity();
   try
   {
      std::vector<int> keywordIdList = m_scriptApi->getKeywordsByCapacity(request.capacity());
      for (std::vector<int>::iterator i = keywordIdList.begin(); i != keywordIdList.end(); ++i)
         answer->add_keywordids(*i);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to GetKeywordsByCapacity request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processWriteKeyword(const pbRequest::WriteKeyword& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   ans.mutable_writekeyword();
   try
   {
      m_scriptApi->writeKeyword(request.keywordid(), request.newstate());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to WriteKeyword request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processSendNotification(const pbRequest::SendNotification& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   ans.mutable_sendnotification();
   try
   {
      m_scriptApi->sendNotification(request.keywordid(), request.recipientid(), request.message());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to SendNotification request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processGetInfo(const pbRequest::GetInfo& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::GetInfo* answer = ans.mutable_getinfo();
   try
   {
      shared::script::yScriptApi::IYScriptApi::EInfoKeys key;
      switch (request.key())
      {
      case pbRequest::GetInfo_Key_kSunrise: key = shared::script::yScriptApi::IYScriptApi::kSunrise; break;
      case pbRequest::GetInfo_Key_kSunset: key = shared::script::yScriptApi::IYScriptApi::kSunset; break;
      case pbRequest::GetInfo_Key_kLatitude: key = shared::script::yScriptApi::IYScriptApi::kLatitude; break;
      case pbRequest::GetInfo_Key_kLongitude: key = shared::script::yScriptApi::IYScriptApi::kLongitude; break;
      case pbRequest::GetInfo_Key_kAltitude: key = shared::script::yScriptApi::IYScriptApi::kAltitude; break;
      case pbRequest::GetInfo_Key_kYadomsServerOS: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerOS; break;
      case pbRequest::GetInfo_Key_kYadomsServerVersion: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerVersion; break;
      default:
         throw shared::exception::CInvalidParameter("answer.waitforeventrequestanswer.type");
      }

      answer->set_value(m_scriptApi->getInfo(key));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to GetInfo request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processGetKeywordName(const pbRequest::GetKeywordName& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::GetKeywordName* answer = ans.mutable_getkeywordname();

   try
   {
      answer->set_keywordname(""); //predefine with "", to ensure answer is complete. Even if an error occurs, it should always return ""
      std::string name = m_scriptApi->getKeywordName(request.keywordid());
      answer->set_keywordname(name);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to GetKeywordName request : " << ex.what();
      throw;
   }
}

void CContextAccessor::processGetKeywordDeviceName(const pbRequest::GetKeywordDeviceName& request, boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   pbAnswer::GetKeywordDeviceName* answer = ans.mutable_getkeyworddevicename();

   try
   {
      answer->set_devicename(""); //predefine with "", to ensure answer is complete. Even if an error occurs, it should always return ""
      std::string name = m_scriptApi->getKeywordDeviceName(request.keywordid());
      answer->set_devicename(name);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
   }

   try
   {
      sendAnswer(ans, messageQueue);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Unable to answer to GetKeywordDeviceName request : " << ex.what();
      throw;
   }
}
