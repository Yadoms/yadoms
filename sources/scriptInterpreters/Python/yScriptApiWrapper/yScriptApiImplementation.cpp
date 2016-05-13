#include "stdafx.h"
#include "yScriptApiImplementation.h"
#include "PythonLibInclude.h"
#include "swigpyrun.h"
#include <shared/DataContainer.h>
#include <shared/currentTime/Provider.h>


CYScriptApiImplementation::CYScriptApiImplementation(const std::string& yScriptApiAccessorId)
{
   // Verify that the version of the library that we linked against is
   // compatible with the version of the headers we compiled against.
   GOOGLE_PROTOBUF_VERIFY_VERSION;

   memset(m_mqBuffer, 0, sizeof(m_mqBuffer));

   try
   {
      const std::string sendMessageQueueId(yScriptApiAccessorId + ".toYadoms");
      const std::string receiveMessageQueueId(yScriptApiAccessorId + ".toScript");
      m_sendMessageQueue    = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, sendMessageQueueId.c_str()   );
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
         throw std::overflow_error("CYScriptApiImplementation::sendRequest : request is not fully initialized");

      if (request.ByteSize() > static_cast<int>(m_messageQueueMessageSize))
         throw std::overflow_error("CYScriptApiImplementation::sendRequest : request is too big");

      if (!request.SerializeToArray(m_mqBuffer, m_messageQueueMessageSize))
         throw std::overflow_error("CYScriptApiImplementation::sendRequest : fail to serialize request (too big ?)");

      m_sendMessageQueue->send(m_mqBuffer, request.GetCachedSize(), 0);
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::overflow_error(std::string("yScriptApiWrapper::sendRequest : Error at IYScriptApi method call, ") + ex.what());
   }
}

void CYScriptApiImplementation::receiveAnswer(pbAnswer::msg& answer) const
{
   // Wait answer
   char message[m_messageQueueMessageSize];
   size_t messageSize;
   unsigned int messagePriority;

   m_receiveMessageQueue->receive(message, m_messageQueueMessageSize, messageSize, messagePriority);

   if (messageSize < 1)
      throw std::runtime_error("yScriptApiWrapper::receiveAnswer : received Yadoms answer is zero length");

   if (!answer.ParseFromArray(message, messageSize))
      throw shared::exception::CInvalidParameter("message");
}

int CYScriptApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
{
   pbRequest::msg req;
   pbRequest::GetKeywordId* request = req.mutable_getkeywordid();
   request->set_devicename(deviceName);
   request->set_keywordname(keywordName);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_getkeywordid())
      throw std::out_of_range("yScriptApiWrapper::getKeywordId, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordId, error : ") + answer.error());

   return answer.getkeywordid().id();
}

int CYScriptApiImplementation::getRecipientId(const std::string& firstName, const std::string& lastName) const
{
   pbRequest::msg req;
   pbRequest::GetRecipientId* request = req.mutable_getrecipientid();
   request->set_firstname(firstName);
   request->set_lastname(lastName);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_getrecipientid())
      throw std::out_of_range("yScriptApiWrapper::getRecipientId, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getRecipientId, error : ") + answer.error());

   return answer.getrecipientid().id();
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   pbRequest::msg req;
   pbRequest::ReadKeyword* request = req.mutable_readkeyword();
   request->set_keywordid(keywordId);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_readkeyword())
      throw std::out_of_range("yScriptApiWrapper::readKeyword, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::readKeyword, error : ") + answer.error());

   return answer.readkeyword().value();
}

std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId, const std::string& timeout) const
{
   pbRequest::msg req;
   pbRequest::WaitForNextAcquisition* request = req.mutable_waitfornextacquisition();
   request->set_keywordid(keywordId);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_waitfornextacquisition())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisition, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisition, error : ") + answer.error());

   return answer.waitfornextacquisition().acquisition();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int> & keywordIdList, const std::string& timeout) const
{
   pbRequest::msg req;
   pbRequest::WaitForNextAcquisitions* request = req.mutable_waitfornextacquisitions();
   for (std::vector<int>::const_iterator it = keywordIdList.begin(); it != keywordIdList.end(); ++it)
      request->add_keywordid(*it);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_waitfornextacquisitions())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisitions, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisitions, error : ") + answer.error());

   return std::pair<int, std::string>(answer.waitfornextacquisitions().keywordid(), answer.waitfornextacquisitions().has_acquisition() ? answer.waitfornextacquisitions().acquisition() : std::string());
}

shared::script::yScriptApi::CWaitForEventResult CYScriptApiImplementation::waitForEvent(const std::vector<int> & keywordIdList, bool receiveDateTimeEvent, const std::string& timeout) const
{
   pbRequest::msg req;
   pbRequest::WaitForEvent* request = req.mutable_waitforevent();
   for (std::vector<int>::const_iterator it = keywordIdList.begin(); it != keywordIdList.end(); ++it)
      request->add_keywordid(*it);
   request->set_receivedatetimeevent(receiveDateTimeEvent);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_waitforevent())
      throw std::out_of_range("yScriptApiWrapper::waitForEvent, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForEvent, error : ") + answer.error());

   shared::script::yScriptApi::CWaitForEventResult result;
   switch (answer.waitforevent().type())
   {
   case pbAnswer::WaitForEvent_EventType_kTimeout : result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout); break;
   case pbAnswer::WaitForEvent_EventType_kKeyword : result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword); break;
   case pbAnswer::WaitForEvent_EventType_kDateTime : result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime); break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
   }

   result.setKeywordId(answer.waitforevent().keywordid());
   result.setValue(answer.waitforevent().has_acquisition() ? answer.waitforevent().acquisition() : std::string());
   return result;
}

std::vector<int> CYScriptApiImplementation::getKeywordsByCapacity(const std::string & capacity) const
{
   pbRequest::msg req;
   pbRequest::GetKeywordsByCapacity * request = req.mutable_getkeywordsbycapacity();
   request->set_capacity(capacity);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordsByCapacity, error : ") + answer.error());

   if (!answer.has_getkeywordsbycapacity())
      throw std::out_of_range("yScriptApiWrapper::getKeywordsByCapacity, wrong message received");

   std::vector<int> keywordIdList;
   for (google::protobuf::RepeatedField<google::protobuf::int32>::const_iterator it = answer.getkeywordsbycapacity().keywordids().begin(); it != answer.getkeywordsbycapacity().keywordids().end(); ++it)
      keywordIdList.push_back(*it);
   return keywordIdList;
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   pbRequest::msg req;
   pbRequest::WriteKeyword* request = req.mutable_writekeyword();
   request->set_keywordid(keywordId);
   request->set_newstate(newState);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_writekeyword())
      throw std::out_of_range("yScriptApiWrapper::writeKeyword, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::writeKeyword, error : ") + answer.error());
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   pbRequest::msg req;
   pbRequest::SendNotification* request = req.mutable_sendnotification();
   request->set_keywordid(keywordId);
   request->set_recipientid(recipientId);
   request->set_message(message);
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_sendnotification())
      throw std::out_of_range("yScriptApiWrapper::sendNotification, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification, error : ") + answer.error());
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   pbRequest::msg req;
   pbRequest::GetInfo* request = req.mutable_getinfo();
   switch (key)
   {
   case kSunrise:request->set_key(pbRequest::GetInfo_Key_kSunrise); break;
   case kSunset:request->set_key(pbRequest::GetInfo_Key_kSunset); break;
   case kLatitude:request->set_key(pbRequest::GetInfo_Key_kLatitude); break;
   case kLongitude:request->set_key(pbRequest::GetInfo_Key_kLongitude); break;
   case kAltitude:request->set_key(pbRequest::GetInfo_Key_kAltitude); break;
   case kYadomsServerOS:request->set_key(pbRequest::GetInfo_Key_kYadomsServerOS); break;
   case kYadomsServerVersion:request->set_key(pbRequest::GetInfo_Key_kYadomsServerVersion); break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
   }
   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_getinfo())
      throw std::out_of_range("yScriptApiWrapper::getInfo, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getInfo, error : ") + answer.error());

   return answer.getinfo().value();
}

std::string CYScriptApiImplementation::getKeywordName(int keywordId) const
{
   pbRequest::msg req;
   pbRequest::GetKeywordName* request = req.mutable_getkeywordname();
   request->set_keywordid(keywordId);

   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_getkeywordname())
      throw std::out_of_range("yScriptApiWrapper::getKeywordName, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordName, error : ") + answer.error());

   return answer.getkeywordname().keywordname();
}

std::string CYScriptApiImplementation::getKeywordDeviceName(int keywordId) const
{
   pbRequest::msg req;
   pbRequest::GetKeywordDeviceName* request = req.mutable_getkeyworddevicename();
   request->set_keywordid(keywordId);

   sendRequest(req);

   pbAnswer::msg answer;
   receiveAnswer(answer);

   if (!answer.has_getkeyworddevicename())
      throw std::out_of_range("yScriptApiWrapper::getKeywordDeviceName, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordDeviceName, error : ") + answer.error());

   return answer.getkeyworddevicename().devicename();
}
