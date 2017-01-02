#include "stdafx.h"
#include "ContextAccessor.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <shared/exception/InvalidParameter.hpp>

const size_t CContextAccessor::m_maxMessages(100);

CContextAccessor::CContextAccessor(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi)
   : CThreadBase(createId()),
     m_scriptApi(yScriptApi),
     m_id(createId()),
     m_readyBarrier(2)
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

   const auto sendMessageQueueId(m_id + ".toScript");
   const auto receiveMessageQueueId(m_id + ".toYadoms");
   const shared::communication::CMessageQueueRemover sendMessageQueueRemover(sendMessageQueueId);
   const shared::communication::CMessageQueueRemover receiveMessageQueueRemover(receiveMessageQueueId);
   try
   {
      std::cout << "Open message queues for id " << m_id << std::endl;
      boost::interprocess::message_queue sendMessageQueue(boost::interprocess::create_only,
                                                          sendMessageQueueId.c_str(),
                                                          m_maxMessages,
                                                          m_messageQueueMessageSize);
      boost::interprocess::message_queue receiveMessageQueue(boost::interprocess::create_only,
                                                             receiveMessageQueueId.c_str(),
                                                             m_maxMessages,
                                                             m_messageQueueMessageSize);

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
            auto messageWasReceived = receiveMessageQueue.timed_receive(message,
                                                                        m_messageQueueMessageSize,
                                                                        messageSize,
                                                                        messagePriority,
                                                                        boost::posix_time::ptime(boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1)));
            boost::this_thread::interruption_point();

            if (messageWasReceived)
               processMessage(message,
                              messageSize,
                              sendMessageQueue);
         }
         catch (shared::exception::CInvalidParameter& ex)
         {
            std::cerr << "Error receiving/processing queue message, invalid parameter : " << ex.what() << std::endl;
         }
         catch (boost::thread_interrupted&)
         {
            break;
         }
         catch (std::exception& ex)
         {
            std::cerr << "Error receiving/processing queue message : " << ex.what() << std::endl;
         }
         catch (...)
         {
            std::cerr << "Error receiving/processing queue message" << std::endl;
         }
      }
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      std::cerr << "Error creating/using message queues (" << m_id << ") in context accessor : " << ex.what() << std::endl;
   }
   catch (std::exception& ex)
   {
      std::cerr << ex.what() << std::endl;
   }
   catch (...)
   {
      std::cerr << "Unknown error" << std::endl;
   }

   std::cout << "Close message queues" << std::endl;

   // Delete all global objects allocated by libprotobuf.
   google::protobuf::ShutdownProtobufLibrary();
}

void CContextAccessor::sendAnswer(const pbAnswer::msg& answer,
                                  boost::interprocess::message_queue& messageQueue)
{
   if (!answer.IsInitialized())
      throw std::overflow_error("CContextAccessor::sendAnswer : answer is not fully initialized");

   if (answer.ByteSize() > static_cast<int>(m_messageQueueMessageSize))
      throw std::overflow_error((boost::format("CContextAccessor::sendAnswer : answer is too big size=%1%, max=%2%")
         % answer.ByteSize() % static_cast<int>(m_messageQueueMessageSize)).str());

   boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);
   if (!answer.SerializeToArray(m_mqBuffer,
                                answer.GetCachedSize()))
      throw std::overflow_error("CContextAccessor::sendAnswer : fail to serialize answer (too big ?)");

   messageQueue.send(m_mqBuffer,
                     answer.GetCachedSize(), 0);
}

void CContextAccessor::processMessage(const void* message,
                                      size_t messageSize,
                                      boost::interprocess::message_queue& messageQueue)
{
   if (messageSize < 1)
      throw shared::exception::CInvalidParameter("messageSize");

   // Unserialize message
   pbRequest::msg request;
   if (!request.ParseFromArray(message,
                               messageSize))
      throw shared::exception::CInvalidParameter("message : fail to parse received data into protobuf format");

   // Process message
   switch (request.OneOf_case())
   {
   case pbRequest::msg::kGetKeywordId: processGetKeywordId(request.getkeywordid(),
                                                           messageQueue);
      break;
   case pbRequest::msg::kGetRecipientId: processGetRecipientId(request.getrecipientid(),
                                                               messageQueue);
      break;
   case pbRequest::msg::kReadKeyword: processReadKeyword(request.readkeyword(),
                                                         messageQueue);
      break;
   case pbRequest::msg::kWaitForNextAcquisition: processWaitForNextAcquisition(request.waitfornextacquisition(),
                                                                               messageQueue);
      break;
   case pbRequest::msg::kWaitForNextAcquisitions: processWaitForNextAcquisitions(request.waitfornextacquisitions(),
                                                                                 messageQueue);
      break;
   case pbRequest::msg::kWaitForEvent: processWaitForEvent(request.waitforevent(),
                                                           messageQueue);
      break;
   case pbRequest::msg::kGetKeywordsByCapacity: processGetKeywordsByCapacity(request.getkeywordsbycapacity(),
                                                                             messageQueue);
      break;
   case pbRequest::msg::kWriteKeyword: processWriteKeyword(request.writekeyword(),
                                                           messageQueue);
      break;
   case pbRequest::msg::kSendNotification: processSendNotification(request.sendnotification(),
                                                                   messageQueue);
      break;
   case pbRequest::msg::kGetInfo: processGetInfo(request.getinfo(),
                                                 messageQueue);
      break;
   case pbRequest::msg::kGetKeywordName: processGetKeywordName(request.getkeywordname(),
                                                               messageQueue);
      break;
   case pbRequest::msg::kGetKeywordDeviceName: processGetKeywordDeviceName(request.getkeyworddevicename(),
                                                                           messageQueue);
      break;
   default:
      throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % request.OneOf_case()).str());
   }
}

void CContextAccessor::processGetKeywordId(const pbRequest::GetKeywordId& request,
                                           boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getkeywordid();
   try
   {
      answer->set_id(m_scriptApi->getKeywordId(request.devicename(),
                                               request.keywordname()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processGetKeywordId request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to GetKeywordId request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processGetRecipientId(const pbRequest::GetRecipientId& request,
                                             boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getrecipientid();
   try
   {
      answer->set_id(m_scriptApi->getRecipientId(request.firstname(),
                                                 request.lastname()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processGetRecipientId request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to GetRecipientId request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processReadKeyword(const pbRequest::ReadKeyword& request,
                                          boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_readkeyword();
   try
   {
      answer->set_value(m_scriptApi->readKeyword(request.keywordid()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processReadKeyword request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to ReadKeyword request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processWaitForNextAcquisition(const pbRequest::WaitForNextAcquisition& request,
                                                     boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_waitfornextacquisition();
   try
   {
      answer->set_acquisition(m_scriptApi->waitForNextAcquisition(request.keywordid(),
                                                                  request.has_timeout() ? request.timeout() : std::string()));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processWaitForNextAcquisition request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to WaitForNextAcquisition request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processWaitForNextAcquisitions(const pbRequest::WaitForNextAcquisitions& request,
                                                      boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_waitfornextacquisitions();
   try
   {
      std::vector<int> keywordIdList;
      for (auto it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
         keywordIdList.push_back(*it);
      auto result = m_scriptApi->waitForNextAcquisitions(keywordIdList,
                                                         request.has_timeout() ? request.timeout() : std::string());
      answer->set_keywordid(result.first);
      if (!result.second.empty())
         answer->set_acquisition(result.second);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processWaitForNextAcquisitions request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to WaitForNextAcquisitions request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processWaitForEvent(const pbRequest::WaitForEvent& request,
                                           boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_waitforevent();
   try
   {
      std::vector<int> keywordIdList;
      for (auto it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
         keywordIdList.push_back(*it);

      auto result = m_scriptApi->waitForEvent(keywordIdList,
                                              request.receivedatetimeevent(),
                                              request.has_timeout() ? request.timeout() : std::string());

      switch (result.getType())
      {
      case shared::script::yScriptApi::CWaitForEventResult::kTimeout: answer->set_type(pbAnswer::WaitForEvent_EventType_kTimeout);
         break;
      case shared::script::yScriptApi::CWaitForEventResult::kKeyword: answer->set_type(pbAnswer::WaitForEvent_EventType_kKeyword);
         break;
      case shared::script::yScriptApi::CWaitForEventResult::kDateTime: answer->set_type(pbAnswer::WaitForEvent_EventType_kDateTime);
         break;
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
      std::cout << "Error processing WaitForEvent request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to WaitForEvent request : " << ex.what() << ". Answer was " << ans.OneOf_case() << std::endl;

      std::stringstream requestDetails;
      requestDetails << "keywordId = { ";
      for (auto it = request.keywordid().begin(); it != request.keywordid().end(); ++it)
         requestDetails << *it << " ";
      requestDetails << "}";
      requestDetails << ", receiveDateTimeEvent = " << (request.receivedatetimeevent() ? "true" : "false");
      if (request.has_timeout())
         requestDetails << ", timeout = " << request.timeout();

      std::cerr << "Request was WaitForEvent(" << requestDetails.str() << ")" << std::endl;
      throw;
   }
}

void CContextAccessor::processGetKeywordsByCapacity(const pbRequest::GetKeywordsByCapacity& request,
                                                    boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getkeywordsbycapacity();
   try
   {
      auto keywordIdList = m_scriptApi->getKeywordsByCapacity(request.capacity());
      for (auto i = keywordIdList.begin(); i != keywordIdList.end(); ++i)
         answer->add_keywordids(*i);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processGetKeywordsByCapacity request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to GetKeywordsByCapacity request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processWriteKeyword(const pbRequest::WriteKeyword& request,
                                           boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   ans.mutable_writekeyword();
   try
   {
      m_scriptApi->writeKeyword(request.keywordid(),
                                request.newstate());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processWriteKeyword request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to WriteKeyword request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processSendNotification(const pbRequest::SendNotification& request,
                                               boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   ans.mutable_sendnotification();
   try
   {
      m_scriptApi->sendNotification(request.keywordid(),
                                    request.recipientid(),
                                    request.message());
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processSendNotification request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to SendNotification request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processGetInfo(const pbRequest::GetInfo& request,
                                      boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getinfo();
   try
   {
      shared::script::yScriptApi::IYScriptApi::EInfoKeys key;
      switch (request.key())
      {
      case pbRequest::GetInfo_Key_kSunrise: key = shared::script::yScriptApi::IYScriptApi::kSunrise;
         break;
      case pbRequest::GetInfo_Key_kSunset: key = shared::script::yScriptApi::IYScriptApi::kSunset;
         break;
      case pbRequest::GetInfo_Key_kLatitude: key = shared::script::yScriptApi::IYScriptApi::kLatitude;
         break;
      case pbRequest::GetInfo_Key_kLongitude: key = shared::script::yScriptApi::IYScriptApi::kLongitude;
         break;
      case pbRequest::GetInfo_Key_kAltitude: key = shared::script::yScriptApi::IYScriptApi::kAltitude;
         break;
      case pbRequest::GetInfo_Key_kYadomsServerOS: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerOS;
         break;
      case pbRequest::GetInfo_Key_kYadomsServerVersion: key = shared::script::yScriptApi::IYScriptApi::kYadomsServerVersion;
         break;
      default:
         throw shared::exception::CInvalidParameter("answer.waitforeventrequestanswer.type");
      }

      answer->set_value(m_scriptApi->getInfo(key));
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processGetInfo request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to GetInfo request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processGetKeywordName(const pbRequest::GetKeywordName& request,
                                             boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getkeywordname();

   try
   {
      answer->set_keywordname(""); //predefine with "", to ensure answer is complete. Even if an error occurs, it should always return ""
      auto name = m_scriptApi->getKeywordName(request.keywordid());
      answer->set_keywordname(name);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processGetKeywordName request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to GetKeywordName request : " << ex.what() << std::endl;
      throw;
   }
}

void CContextAccessor::processGetKeywordDeviceName(const pbRequest::GetKeywordDeviceName& request,
                                                   boost::interprocess::message_queue& messageQueue)
{
   pbAnswer::msg ans;
   auto answer = ans.mutable_getkeyworddevicename();

   try
   {
      answer->set_devicename(""); //predefine with "", to ensure answer is complete. Even if an error occurs, it should always return ""
      auto name = m_scriptApi->getKeywordDeviceName(request.keywordid());
      answer->set_devicename(name);
   }
   catch (std::exception& ex)
   {
      ans.set_error(ex.what());
      std::cout << "Error processing processGetKeywordDeviceName request : " << ex.what() << std::endl;
   }

   try
   {
      sendAnswer(ans,
                 messageQueue);
   }
   catch (std::exception& ex)
   {
      std::cerr << "Unable to answer to GetKeywordDeviceName request : " << ex.what() << std::endl;
      throw;
   }
}
