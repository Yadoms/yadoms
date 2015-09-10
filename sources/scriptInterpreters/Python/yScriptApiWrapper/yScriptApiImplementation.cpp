#include "stdafx.h"
#include "yScriptApiImplementation.h"
#include "PythonLibInclude.h"
#include "swigpyrun.h"
#include <shared/DataContainer.h>


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

void CYScriptApiImplementation::receiveAnswer(EAnswerIdentifier expectedAnswerId, shared::CDataContainer& answer, const boost::posix_time::time_duration& timeout) const
{
   if (!tryReceiveAnswer(expectedAnswerId, answer, timeout))
      throw std::runtime_error("yScriptApiWrapper::receiveAnswer : Timeout waiting for Yadoms answer");
}

bool CYScriptApiImplementation::tryReceiveAnswer(EAnswerIdentifier expectedAnswerId, shared::CDataContainer& answer, const boost::posix_time::time_duration& timeout) const
{
   // Wait answer
   char message[m_messageQueueMessageSize];
   size_t messageSize;
   unsigned int messagePriority;

   // Add a small timeout to let time to Yadoms to answer
   if (!m_receiveMessageQueue->timed_receive(message, m_messageQueueMessageSize, messageSize, messagePriority, boost::posix_time::second_clock::universal_time() + boost::posix_time::seconds(10) + timeout))
      return false;

   if (messageSize < 1)
      throw std::runtime_error("yScriptApiWrapper::tryReceiveAnswer : received Yadoms answer is zero length");

   // Unserialize received message
   shared::CDataContainer mainAnswerContainer(std::string(message, messageSize));

   if (!mainAnswerContainer.exists("type") || !mainAnswerContainer.exists("content"))
      throw std::out_of_range("yScriptApiWrapper::tryReceiveAnswer : received Yadoms answer is not well formed");

   if (mainAnswerContainer.get<EAnswerIdentifier>("type") != expectedAnswerId)
      throw std::out_of_range("yScriptApiWrapper::tryReceiveAnswer : received Yadoms answer is wrong type");

   answer = mainAnswerContainer.get<shared::CDataContainer>("content");

   return true;
}


int CYScriptApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
{
   shared::CDataContainer request;
   request.set("device", deviceName);
   request.set("keyword", keywordName);
   sendRequest(kReqGetKeywordId, request);

   shared::CDataContainer answer;
   receiveAnswer(kAnsGetKeywordId, answer);

   return answer.get<int>("returnValue");
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   sendRequest(kReqReadKeyword, request);

   shared::CDataContainer answer;
   receiveAnswer(kAnsReadKeyword, answer);

   return answer.get<std::string>("returnValue");
}

std::string CYScriptApiImplementation::waitForAcquisition(int keywordId, const std::string& timeout) const
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   request.set("timeout", timeout);
   sendRequest(kReqWaitForAcquisition, request);
   shared::CDataContainer answer;
   if (!tryReceiveAnswer(kAnsWaitForAcquisition, answer, timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout)))
      return std::string();

   return answer.get<std::string>("returnValue");
}

std::pair<int, std::string> CYScriptApiImplementation::waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const
{
   shared::CDataContainer request;
   request.set("keywordIdList", keywordIdList);
   request.set("timeout", timeout);
   sendRequest(kReqWaitForAcquisitions, request);
   shared::CDataContainer answer;
   if (!tryReceiveAnswer(kAnsWaitForAcquisitions, answer, timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout)))
      return std::pair<int, std::string>();

   return std::make_pair(answer.get<int>("key"), answer.get<std::string>("value"));
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   request.set("newState", newState);
   sendRequest(kReqWriteKeyword, request);
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   shared::CDataContainer request;
   request.set("keywordId", keywordId);
   request.set("recipientId", recipientId);
   request.set("message", message);
   sendRequest(kReqSendNotification, request);
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   shared::CDataContainer request;
   request.set("key", key);
   sendRequest(kReqGetInfo, request);

   shared::CDataContainer answer;
   receiveAnswer(kAnsGetInfo, answer, boost::posix_time::seconds(30)); // Ehanced timeout for certains long requests

   return answer.get<std::string>("returnValue");
}

void CYScriptApiImplementation::ruleEnable(bool enable)
{
   shared::CDataContainer request;
   request.set("enable", enable);
   sendRequest(kReqRuleEnable, request);
}