#include "stdafx.h"
#include "ApiImplementation.h"
#include <shared/DataContainer.h>
#include <shared/communication/SmallHeaderMessageCutter.h>
#include <shared/communication/SmallHeaderMessageAssembler.h>

shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId)
{
   return new CYScriptApiImplementation(yScriptApiAccessorId);
}

void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi)
{
   delete yApi;
}


CYScriptApiImplementation::CYScriptApiImplementation(const std::string& yScriptApiAccessorId)
{
   // Verify that the version of the library that we linked against is
   // compatible with the version of the headers we compiled against.
   GOOGLE_PROTOBUF_VERIFY_VERSION;

   try
   {
      const auto sendMessageQueueId(yScriptApiAccessorId + ".script_IPC.toYadoms");
      const auto receiveMessageQueueId(yScriptApiAccessorId + ".script_IPC.toScript");
      m_sendMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, sendMessageQueueId.c_str());
      m_receiveMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, receiveMessageQueueId.c_str());
      m_messageCutter = boost::make_shared<shared::communication::SmallHeaderMessageCutter>(m_sendMessageQueue->get_max_msg_size(),
                                                                                            m_sendMessageQueue->get_max_msg());
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

void CYScriptApiImplementation::sendRequest(const script_IPC::toYadoms::msg& request) const
{
   try
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);

      if (!m_sendMessageQueue || !m_messageCutter)
         throw std::runtime_error((boost::format("CYScriptApiImplementation::send \"%1%\", script API not ready to send message") % request.descriptor()->full_name()).str());

      if (!request.IsInitialized())
         throw std::runtime_error((boost::format("CYScriptApiImplementation::send \"%1%\", request is not fully initialized") % request.descriptor()->full_name()).str());

      const auto pbMessageSize = request.ByteSize();
      const auto serializedMessage = boost::make_shared<unsigned char[]>(pbMessageSize);
      if (!request.SerializeWithCachedSizesToArray(serializedMessage.get()))
         throw std::runtime_error((boost::format("CYScriptApiImplementation::send \"%1%\", fail to serialize request (too big ?)") % request.descriptor()->full_name()).str());

      const auto cuttedMessage = m_messageCutter->cut(serializedMessage,
                                                      pbMessageSize);

      if (!cuttedMessage->empty())
      {
         for (const auto& part : *cuttedMessage)
         {
            m_sendMessageQueue->send(part->formattedMessage(),
                                     part->formattedSize(),
                                     0);
         }
      }
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::overflow_error((boost::format("yScriptApiWrapper::sendRequest \"%1%\" : Error at IYScriptApi method call, %2%") % request.descriptor()->full_name() % ex.what()).str());
   }
}

void CYScriptApiImplementation::receiveAnswer(script_IPC::toScript::msg& answer) const
{
   // Wait answer
   auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue->get_max_msg_size()));
   size_t messageSize;
   unsigned int messagePriority;
   const auto messageAssembler = boost::make_shared<shared::communication::SmallHeaderMessageAssembler>(m_receiveMessageQueue->get_max_msg_size());

   while (!messageAssembler->isCompleted())
   {
      m_receiveMessageQueue->receive(message.get(),
                                     m_receiveMessageQueue->get_max_msg_size(),
                                     messageSize,
                                     messagePriority);

      messageAssembler->appendPart(message,
                                   messageSize);
   }

   if (messageAssembler->messageSize() < 1)
      throw std::runtime_error("CYScriptApiImplementation::receiveAnswer : received Yadoms answer is zero length");

   if (!answer.ParseFromArray(messageAssembler->message().get(),
                              messageAssembler->messageSize()))
      throw shared::exception::CInvalidParameter("message : fail to parse received answer data into protobuf format");
}

int CYScriptApiImplementation::getKeywordId(const std::string& deviceName,
                                            const std::string& keywordName) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_getkeywordid();
   request->set_devicename(deviceName);
   request->set_keywordname(keywordName);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordId returned error : ") + answer.error());

   if (!answer.has_getkeywordid())
      throw std::out_of_range("yScriptApiWrapper::getKeywordId, wrong message received");

   return answer.getkeywordid().id();
}

int CYScriptApiImplementation::getRecipientId(const std::string& firstName,
                                              const std::string& lastName) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_getrecipientid();
   request->set_firstname(firstName);
   request->set_lastname(lastName);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::getRecipientId returned error : ") + answer.error());

   if (!answer.has_getrecipientid())
      throw std::out_of_range("yScriptApiWrapper::getRecipientId, wrong message received");

   return answer.getrecipientid().id();
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_readkeyword();
   request->set_keywordid(keywordId);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::readKeyword returned error : ") + answer.error());

   if (!answer.has_readkeyword())
      throw std::out_of_range("yScriptApiWrapper::readKeyword, wrong message received");

   return answer.readkeyword().value();
}

std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId,
                                                              const std::string& timeout) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_waitfornextacquisition();
   request->set_keywordid(keywordId);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisition returned error : ") + answer.error());

   if (!answer.has_waitfornextacquisition())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisition, wrong message received");

   return answer.waitfornextacquisition().acquisition();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int>& keywordIdList,
                                                                               const std::string& timeout) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_waitfornextacquisitions();
   for (const auto it : keywordIdList)
      request->add_keywordid(it);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisitions returned error : ") + answer.error());

   if (!answer.has_waitfornextacquisitions())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisitions, wrong message received");

   return std::pair<int, std::string>(answer.waitfornextacquisitions().keywordid(),
                                      answer.waitfornextacquisitions().acquisition());
}

shared::script::yScriptApi::CWaitForEventResult CYScriptApiImplementation::waitForEvent(const std::vector<int>& keywordIdList,
                                                                                        bool receiveDateTimeEvent,
                                                                                        const std::string& timeout) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_waitforevent();
   for (const auto it : keywordIdList)
      request->add_keywordid(it);
   request->set_receivedatetimeevent(receiveDateTimeEvent);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForEvent returned error : ") + answer.error());

   if (!answer.has_waitforevent())
      throw std::out_of_range("yScriptApiWrapper::waitForEvent, wrong message received");

   shared::script::yScriptApi::CWaitForEventResult result;
   switch (answer.waitforevent().type())
   {
   case script_IPC::toScript::WaitForEvent_EventType_kTimeout: result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout);
      break;
   case script_IPC::toScript::WaitForEvent_EventType_kKeyword: result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword);
      break;
   case script_IPC::toScript::WaitForEvent_EventType_kDateTime: result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime);
      break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
   }

   result.setKeywordId(answer.waitforevent().keywordid());
   result.setValue(answer.waitforevent().acquisition());
   return result;
}

std::vector<int> CYScriptApiImplementation::getKeywordsByCapacity(const std::string& capacity) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_getkeywordsbycapacity();
   request->set_capacity(capacity);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
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
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_writekeyword();
   request->set_keywordid(keywordId);
   request->set_newstate(newState);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::writeKeyword returned error : ") + answer.error());

   if (!answer.has_writekeyword())
      throw std::out_of_range("yScriptApiWrapper::writeKeyword, wrong message received");
}

void CYScriptApiImplementation::sendNotification(int keywordId,
                                                 int recipientId,
                                                 const std::string& message)
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_sendnotification();
   request->set_keywordid(keywordId);
   request->set_recipientid(recipientId);
   request->set_message(message);
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification returned error : ") + answer.error());

   if (!answer.has_sendnotification())
      throw std::out_of_range("yScriptApiWrapper::sendNotification, wrong message received");
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_getinfo();
   switch (key)
   {
   case kSunrise: request->set_key(script_IPC::toYadoms::GetInfo_Key_kSunrise);
      break;
   case kSunset: request->set_key(script_IPC::toYadoms::GetInfo_Key_kSunset);
      break;
   case kLatitude: request->set_key(script_IPC::toYadoms::GetInfo_Key_kLatitude);
      break;
   case kLongitude: request->set_key(script_IPC::toYadoms::GetInfo_Key_kLongitude);
      break;
   case kAltitude: request->set_key(script_IPC::toYadoms::GetInfo_Key_kAltitude);
      break;
   case kYadomsServerOS: request->set_key(script_IPC::toYadoms::GetInfo_Key_kYadomsServerOS);
      break;
   case kYadomsServerVersion: request->set_key(script_IPC::toYadoms::GetInfo_Key_kYadomsServerVersion);
      break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
   }
   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::getInfo returned error : ") + answer.error());

   if (!answer.has_getinfo())
      throw std::out_of_range("yScriptApiWrapper::getInfo, wrong message received");

   return answer.getinfo().value();
}

std::string CYScriptApiImplementation::getKeywordName(int keywordId) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_getkeywordname();
   request->set_keywordid(keywordId);

   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordName returned error : ") + answer.error());

   if (!answer.has_getkeywordname())
      throw std::out_of_range("yScriptApiWrapper::getKeywordName, wrong message received");

   return answer.getkeywordname().keywordname();
}

std::string CYScriptApiImplementation::getKeywordDeviceName(int keywordId) const
{
   script_IPC::toYadoms::msg req;
   auto request = req.mutable_getkeyworddevicename();
   request->set_keywordid(keywordId);

   sendRequest(req);

   script_IPC::toScript::msg answer;
   receiveAnswer(answer);

   if (!answer.error().empty())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordDeviceName returned error : ") + answer.error());

   if (!answer.has_getkeyworddevicename())
      throw std::out_of_range("yScriptApiWrapper::getKeywordDeviceName, wrong message received");

   return answer.getkeyworddevicename().devicename();
}

