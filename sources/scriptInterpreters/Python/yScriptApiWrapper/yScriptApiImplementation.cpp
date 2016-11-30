#include "stdafx.h"
#include "yScriptApiImplementation.h"
#include <shared/DataContainer.h>


CYScriptApiImplementation::CYScriptApiImplementation(const std::string& yScriptApiAccessorId)
{
   // Verify that the version of the library that we linked against is
   // compatible with the version of the headers we compiled against.
   GOOGLE_PROTOBUF_VERIFY_VERSION;

   memset(m_mqBuffer, 0, sizeof(m_mqBuffer));

   try
   {
      const auto sendMessageQueueId(yScriptApiAccessorId + ".toYadoms");
      const auto receiveMessageQueueId(yScriptApiAccessorId + ".toScript");
      m_sendMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, sendMessageQueueId.c_str());
      m_receiveMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, receiveMessageQueueId.c_str());
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::runtime_error(std::string("yScriptApiWrapper::yScriptApiWrapper : Error opening message queue, ") + ex.what());
   }
   catch (...)
   {
      throw std::runtime_error("yScriptApiWrapper::yScriptApiWrapper : Unknown error opening message queue");
   }
}

CYScriptApiImplementation::~CYScriptApiImplementation()
{
   // Delete all global objects allocated by libprotobuf.
   google::protobuf::ShutdownProtobufLibrary();
}

void CYScriptApiImplementation::sendRequest(const pbRequest::msg& request) const
{
   try
   {
      if (!request.IsInitialized())
         throw std::overflow_error((boost::format("CYScriptApiImplementation::sendRequest \"%1%\" : request is not fully initialized") % request.descriptor()->full_name()).str());

      if (request.ByteSize() > static_cast<int>(m_messageQueueMessageSize))
         throw std::overflow_error((boost::format("CYScriptApiImplementation::sendRequest \"%1%\" : request is too big") % request.descriptor()->full_name()).str());

      boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);
      if (!request.SerializeToArray(m_mqBuffer, request.GetCachedSize()))
         throw std::overflow_error((boost::format("CYScriptApiImplementation::sendRequest \"%1%\" : fail to serialize request (too big ?)") % request.descriptor()->full_name()).str());

      m_sendMessageQueue->send(m_mqBuffer,
                               request.GetCachedSize(),
                               0);
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::overflow_error((boost::format("yScriptApiWrapper::sendRequest \"%1%\" : Error at IYScriptApi method call, %2%") % request.descriptor()->full_name() % ex.what()).str());
   }
}

void CYScriptApiImplementation::receiveAnswer(pbAnswer::msg& answer) const
{
   // Wait answer
   char message[m_messageQueueMessageSize];
   size_t messageSize;
   unsigned int messagePriority;

   m_receiveMessageQueue->receive(message,
                                  m_messageQueueMessageSize,
                                  messageSize,
                                  messagePriority);

   if (messageSize < 1)
      throw std::runtime_error("yScriptApiWrapper::receiveAnswer : received Yadoms answer is zero length");

   if (!answer.ParseFromArray(message, messageSize))
      throw shared::exception::CInvalidParameter("message : fail to parse received answer data into protobuf format");
}

int CYScriptApiImplementation::getKeywordId(const std::string& deviceName,
                                            const std::string& keywordName) const
{
   pbRequest::msg req;
   auto request = req.mutable_getkeywordid();
   request->set_devicename(deviceName);
   request->set_keywordname(keywordName);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordId returned error : ") + answer.error());

   if (!answer.has_getkeywordid())
      throw std::out_of_range("yScriptApiWrapper::getKeywordId, wrong message received");

   return answer.getkeywordid().id();
}

int CYScriptApiImplementation::getRecipientId(const std::string& firstName,
                                              const std::string& lastName) const
{
   pbRequest::msg req;
   auto request = req.mutable_getrecipientid();
   request->set_firstname(firstName);
   request->set_lastname(lastName);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getRecipientId returned error : ") + answer.error());

   if (!answer.has_getrecipientid())
      throw std::out_of_range("yScriptApiWrapper::getRecipientId, wrong message received");

   return answer.getrecipientid().id();
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   pbRequest::msg req;
   auto request = req.mutable_readkeyword();
   request->set_keywordid(keywordId);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::readKeyword returned error : ") + answer.error());

   if (!answer.has_readkeyword())
      throw std::out_of_range("yScriptApiWrapper::readKeyword, wrong message received");

   return answer.readkeyword().value();
}

std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId,
                                                              const std::string& timeout) const
{
   pbRequest::msg req;
   auto request = req.mutable_waitfornextacquisition();
   request->set_keywordid(keywordId);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisition returned error : ") + answer.error());

   if (!answer.has_waitfornextacquisition())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisition, wrong message received");

   return answer.waitfornextacquisition().acquisition();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int>& keywordIdList,
                                                                               const std::string& timeout) const
{
   pbRequest::msg req;
   auto request = req.mutable_waitfornextacquisitions();
   for (const auto it : keywordIdList)
      request->add_keywordid(it);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisitions returned error : ") + answer.error());

   if (!answer.has_waitfornextacquisitions())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisitions, wrong message received");

   return std::pair<int, std::string>(answer.waitfornextacquisitions().keywordid(), answer.waitfornextacquisitions().has_acquisition() ? answer.waitfornextacquisitions().acquisition() : std::string());
}

shared::script::yScriptApi::CWaitForEventResult CYScriptApiImplementation::waitForEvent(const std::vector<int>& keywordIdList,
                                                                                        bool receiveDateTimeEvent,
                                                                                        const std::string& timeout) const
{
   pbRequest::msg req;
   auto request = req.mutable_waitforevent();
   for (const auto it : keywordIdList)
      request->add_keywordid(it);
   request->set_receivedatetimeevent(receiveDateTimeEvent);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForEvent returned error : ") + answer.error());

   if (!answer.has_waitforevent())
      throw std::out_of_range("yScriptApiWrapper::waitForEvent, wrong message received");

   shared::script::yScriptApi::CWaitForEventResult result;
   switch (answer.waitforevent().type())
   {
   case pbAnswer::WaitForEvent_EventType_kTimeout: result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout);
      break;
   case pbAnswer::WaitForEvent_EventType_kKeyword: result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword);
      break;
   case pbAnswer::WaitForEvent_EventType_kDateTime: result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime);
      break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
   }

   result.setKeywordId(answer.waitforevent().keywordid());
   result.setValue(answer.waitforevent().has_acquisition() ? answer.waitforevent().acquisition() : std::string());
   return result;
}

std::vector<int> CYScriptApiImplementation::getKeywordsByCapacity(const std::string& capacity) const
{
   pbRequest::msg req;
   auto request = req.mutable_getkeywordsbycapacity();
   request->set_capacity(capacity);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordsByCapacity, error : ") + answer.error());

   if (!answer.has_getkeywordsbycapacity())
      throw std::out_of_range("yScriptApiWrapper::getKeywordsByCapacity, wrong message received");

   std::vector<int> keywordIdList;
   for (const auto it : answer.getkeywordsbycapacity().keywordids())
      keywordIdList.push_back(it);
   return keywordIdList;
}

void CYScriptApiImplementation::writeKeyword(int keywordId,
                                             const std::string& newState)
{
   pbRequest::msg req;
   auto request = req.mutable_writekeyword();
   request->set_keywordid(keywordId);
   request->set_newstate(newState);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::writeKeyword returned error : ") + answer.error());

   if (!answer.has_writekeyword())
      throw std::out_of_range("yScriptApiWrapper::writeKeyword, wrong message received");
}

void CYScriptApiImplementation::sendNotification(int keywordId,
                                                 int recipientId,
                                                 const std::string& message)
{
   pbRequest::msg req;
   auto request = req.mutable_sendnotification();
   request->set_keywordid(keywordId);
   request->set_recipientid(recipientId);
   request->set_message(message);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification returned error : ") + answer.error());

   if (!answer.has_sendnotification())
      throw std::out_of_range("yScriptApiWrapper::sendNotification, wrong message received");
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   pbRequest::msg req;
   auto request = req.mutable_getinfo();
   switch (key)
   {
   case kSunrise: request->set_key(pbRequest::GetInfo_Key_kSunrise);
      break;
   case kSunset: request->set_key(pbRequest::GetInfo_Key_kSunset);
      break;
   case kLatitude: request->set_key(pbRequest::GetInfo_Key_kLatitude);
      break;
   case kLongitude: request->set_key(pbRequest::GetInfo_Key_kLongitude);
      break;
   case kAltitude: request->set_key(pbRequest::GetInfo_Key_kAltitude);
      break;
   case kYadomsServerOS: request->set_key(pbRequest::GetInfo_Key_kYadomsServerOS);
      break;
   case kYadomsServerVersion: request->set_key(pbRequest::GetInfo_Key_kYadomsServerVersion);
      break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
   }
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getInfo returned error : ") + answer.error());

   if (!answer.has_getinfo())
      throw std::out_of_range("yScriptApiWrapper::getInfo, wrong message received");

   return answer.getinfo().value();
}

std::string CYScriptApiImplementation::getKeywordName(int keywordId) const
{
   pbRequest::msg req;
   auto request = req.mutable_getkeywordname();
   request->set_keywordid(keywordId);

   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordName returned error : ") + answer.error());

   if (!answer.has_getkeywordname())
      throw std::out_of_range("yScriptApiWrapper::getKeywordName, wrong message received");

   return answer.getkeywordname().keywordname();
}

std::string CYScriptApiImplementation::getKeywordDeviceName(int keywordId) const
{
   pbRequest::msg req;
   auto request = req.mutable_getkeyworddevicename();
   request->set_keywordid(keywordId);

   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordDeviceName returned error : ") + answer.error());

   if (!answer.has_getkeyworddevicename())
      throw std::out_of_range("yScriptApiWrapper::getKeywordDeviceName, wrong message received");

   return answer.getkeyworddevicename().devicename();
}

