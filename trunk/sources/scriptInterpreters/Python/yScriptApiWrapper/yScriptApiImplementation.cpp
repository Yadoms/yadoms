#include "stdafx.h"
#include "yScriptApiImplementation.h"
#include "PythonLibInclude.h"
#include "swigpyrun.h"
#include <shared/DataContainer.h>
#include <shared/currentTime/Provider.h>


CYScriptApiImplementation::CYScriptApiImplementation(const std::string& yScriptApiAccessorId)
{
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
}

void CYScriptApiImplementation::sendRequest(ERequestIdentifier requestId, const shared::CDataContainer& request) const
{
   try
   {
      shared::CDataContainer mainRequestContainer;
      mainRequestContainer.set("type", requestId);
      mainRequestContainer.set("content", request);

      const std::string serializedRequest(mainRequestContainer.serialize());
      if (serializedRequest.size() > m_messageQueueMessageSize)
         throw std::overflow_error("yScriptApiWrapper::sendRequest : request is too big");
      m_sendMessageQueue->send(serializedRequest.c_str(), serializedRequest.size(), 0);
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::overflow_error(std::string("yScriptApiWrapper::sendRequest : Error at IYScriptApi method call, ") + ex.what());
   }
}

shared::CDataContainer CYScriptApiImplementation::receiveAnswer(EAnswerIdentifier expectedAnswerId) const
{
   // Wait answer
   char message[m_messageQueueMessageSize];
   size_t messageSize;
   unsigned int messagePriority;

   m_receiveMessageQueue->receive(message, m_messageQueueMessageSize, messageSize, messagePriority);

   if (messageSize < 1)
      throw std::runtime_error("yScriptApiWrapper::receiveAnswer : received Yadoms answer is zero length");

   // Unserialize received message
   shared::CDataContainer mainAnswerContainer(std::string(message, messageSize));

   if (!mainAnswerContainer.exists("type") || !mainAnswerContainer.exists("content"))
      throw std::out_of_range("yScriptApiWrapper::receiveAnswer : received Yadoms answer is not well formed");

   if (mainAnswerContainer.get<EAnswerIdentifier>("type") != expectedAnswerId)
      throw std::out_of_range("yScriptApiWrapper::receiveAnswer : received Yadoms answer is wrong type");

   return mainAnswerContainer.get<shared::CDataContainer>("content");
}

int CYScriptApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
{
   shared::CDataContainer request;
   request.set("device", deviceName);
   request.set("keyword", keywordName);
   sendRequest(kReqGetKeywordId, request);

   shared::CDataContainer answer = receiveAnswer(kAnsGetKeywordId);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordId, error : ") + answer.get<std::string>("error"));

   return answer.get<int>("returnValue");
}

int CYScriptApiImplementation::getRecipientId(const std::string& firstName, const std::string& lastName) const
{
   shared::CDataContainer request;
   request.set("firstName", firstName);
   request.set("lastName", lastName);
   sendRequest(kReqGetRecipientId, request);

   shared::CDataContainer answer = receiveAnswer(kAnsGetRecipientId);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::getRecipientId, error : ") + answer.get<std::string>("error"));

   return answer.get<int>("returnValue");
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   sendRequest(kReqReadKeyword, request);

   shared::CDataContainer answer = receiveAnswer(kAnsReadKeyword);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::readKeyword, error : ") + answer.get<std::string>("error"));

   return answer.get<std::string>("returnValue");
}

std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId, const std::string& timeout) const
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   request.set("timeout", timeout);
   sendRequest(kReqWaitForNextAcquisition, request);
   shared::CDataContainer answer = receiveAnswer(kAnsWaitForAcquisition);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisition, error : ") + answer.get<std::string>("error"));

   return answer.get<std::string>("returnValue");
}

std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const
{
   shared::CDataContainer request;
   request.set("keywordIdList", keywordIdList);
   request.set("timeout", timeout);
   sendRequest(kReqWaitForNextAcquisitions, request);
   shared::CDataContainer answer = receiveAnswer(kAnsWaitForAcquisitions);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisitions, error : ") + answer.get<std::string>("error"));

   return std::make_pair(answer.get<int>("key"), answer.get<std::string>("value"));
}

void CYScriptApiImplementation::wait(const std::string& dateTimeOrDuration) const
{
   shared::CDataContainer request;
   request.set("dateTimeOrDuration", dateTimeOrDuration);
   sendRequest(kReqWait, request);

   shared::CDataContainer answer = receiveAnswer(kAnsWait);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::wait, error : ") + answer.get<std::string>("error"));
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   request.set("newState", newState);
   sendRequest(kReqWriteKeyword, request);

   shared::CDataContainer answer = receiveAnswer(kAnsWriteKeyword);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::writeKeyword, error : ") + answer.get<std::string>("error"));
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   request.set("recipientId", recipientId);
   request.set("message", message);
   sendRequest(kReqSendNotification, request);

   shared::CDataContainer answer = receiveAnswer(kAnsSendNotification);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification, error : ") + answer.get<std::string>("error"));
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   shared::CDataContainer request;
   request.set("key", key);
   sendRequest(kReqGetInfo, request);

   shared::CDataContainer answer = receiveAnswer(kAnsGetInfo);

   if (answer.exists("error"))
      throw std::out_of_range(std::string("yScriptApiWrapper::getInfo, error : ") + answer.get<std::string>("error"));

   return answer.get<std::string>("returnValue");
}