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

void CYScriptApiImplementation::sendRequest(const google::protobuf::Message& request) const
{
   try
   {
      unsigned char message[m_messageQueueMessageSize]; //TODO à mettre dans la classe pour ne pas allouer/désallouer sans cesse

      if (!request.IsInitialized())
         throw std::overflow_error("CYScriptApiImplementation::sendRequest : request is not fully initialized");

      if (request.ByteSize() > m_messageQueueMessageSize)
         throw std::overflow_error("CYScriptApiImplementation::sendRequest : request is too big");

      if (!request.SerializeToArray(message, m_messageQueueMessageSize))
         throw std::overflow_error("CYScriptApiImplementation::sendRequest : fail to serialize request (too big ?)");

      m_sendMessageQueue->send(message, request.GetCachedSize(), 0);
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::overflow_error(std::string("yScriptApiWrapper::sendRequest : Error at IYScriptApi method call, ") + ex.what());
   }
}

void CYScriptApiImplementation::receiveAnswer(protobufMessage::ToScript& answer) const
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
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_GetKeywordIdRequest* request = req.mutable_getkeywordidrequest();
   request->set_devicename(deviceName);
   request->set_keywordname(keywordName);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_getkeywordidanswer())
      throw std::out_of_range("yScriptApiWrapper::getKeywordId, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordId, error : ") + answer.error());

   return answer.getkeywordidanswer().id();
}

int CYScriptApiImplementation::getRecipientId(const std::string& firstName, const std::string& lastName) const
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_GetRecipientIdRequest* request = req.mutable_getrecipientidrequest();
   request->set_firstname(firstName);
   request->set_lastname(lastName);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_getrecipientidanswer())
      throw std::out_of_range("yScriptApiWrapper::getRecipientId, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::getRecipientId, error : ") + answer.error());

   return answer.getrecipientidanswer().id();
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_ReadKeywordRequest* request = req.mutable_readkeywordrequest();
   request->set_keywordid(keywordId);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_readkeywordanswer())
      throw std::out_of_range("yScriptApiWrapper::readKeyword, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::readKeyword, error : ") + answer.error());

   return answer.readkeywordanswer().value();
}

std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId, const std::string& timeout) const
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_WaitForNextAcquisitionRequest* request = req.mutable_waitfornextacquisitionrequest();
   request->set_keywordid(keywordId);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_waitfornextacquisitionanswer())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisition, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisition, error : ") + answer.error());

   return answer.waitfornextacquisitionanswer().acquisition();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_WaitForNextAcquisitionsRequest* request = req.mutable_waitfornextacquisitionsrequest();
   for (std::vector<int>::const_iterator it = keywordIdList.begin(); it != keywordIdList.end(); ++it)
      request->add_keywordid(*it);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_waitfornextacquisitionsanswer())
      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisitions, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisitions, error : ") + answer.error());

   return std::pair<int, std::string>(answer.waitfornextacquisitionsanswer().keywordid(), answer.waitfornextacquisitionsanswer().has_acquisition() ? answer.waitfornextacquisitionsanswer().acquisition() : std::string());
}

shared::script::yScriptApi::CWaitForEventResult CYScriptApiImplementation::waitForEvent(const std::vector<int> keywordIdList, bool receiveDateTimeEvent, const std::string& timeout) const
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_WaitForEventRequest* request = req.mutable_waitforeventrequest();
   for (std::vector<int>::const_iterator it = keywordIdList.begin(); it != keywordIdList.end(); ++it)
      request->add_keywordid(*it);
   if (!timeout.empty())
      request->set_timeout(timeout);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_waitforeventrequestanswer())
      throw std::out_of_range("yScriptApiWrapper::waitForEvent, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForEvent, error : ") + answer.error());

   shared::script::yScriptApi::CWaitForEventResult result;
   switch (answer.waitforeventrequestanswer().type())
   {
   case protobufMessage::ToScript_WaitForEventAnswer_EventType::ToScript_WaitForEventAnswer_EventType_kTimeout:result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout); break;
   case protobufMessage::ToScript_WaitForEventAnswer_EventType::ToScript_WaitForEventAnswer_EventType_kKeyword:result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword); break;
   case protobufMessage::ToScript_WaitForEventAnswer_EventType::ToScript_WaitForEventAnswer_EventType_kDateTime:result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime); break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforeventrequestanswer.type");
   }

   result.setKeywordId(answer.waitforeventrequestanswer().keywordid());
   result.setValue(answer.waitforeventrequestanswer().has_acquisition() ? answer.waitforeventrequestanswer().acquisition() : std::string());
   return result;
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_WriteKeywordRequest* request = req.mutable_writekeywordrequest();
   request->set_keywordid(keywordId);//TODO utiliser des lambdas
   request->set_newstate(newState);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_writekeywordanswer())
      throw std::out_of_range("yScriptApiWrapper::writeKeyword, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::writeKeyword, error : ") + answer.error());
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_SendNotificationRequest* request = req.mutable_sendnotificationrequest();
   request->set_keywordid(keywordId);
   request->set_recipientid(recipientId);
   request->set_message(message);
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_sendnotificationanswer())
      throw std::out_of_range("yScriptApiWrapper::sendNotification, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification, error : ") + answer.error());
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   protobufMessage::ToYadoms req;
   protobufMessage::ToYadoms_GetInfoRequest* request = req.mutable_getinforequest();
   switch (key)
   {
   case kSunrise:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kSunrise); break;
   case kSunset:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kSunset); break;
   case kLatitude:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kLatitude); break;
   case kLongitude:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kLongitude); break;
   case kAltitude:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kAltitude); break;
   case kYadomsServerOS:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kYadomsServerOS); break;
   case kYadomsServerVersion:request->set_key(protobufMessage::ToYadoms_GetInfoRequest_Key::ToYadoms_GetInfoRequest_Key_kYadomsServerVersion); break;
   default:
      throw shared::exception::CInvalidParameter("answer.waitforeventrequestanswer.type");
   }
   sendRequest(req);

   protobufMessage::ToScript answer;
   receiveAnswer(answer);

   if (!answer.has_sendnotificationanswer())
      throw std::out_of_range("yScriptApiWrapper::sendNotification, wrong message received");

   if (answer.has_error())
      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification, error : ") + answer.error());

   return answer.getinfoanswer().value();
}