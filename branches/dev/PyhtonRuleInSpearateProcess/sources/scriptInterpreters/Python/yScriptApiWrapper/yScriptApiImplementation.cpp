#include "stdafx.h"
#include "yScriptApiImplementation.h"
#include "PythonLibInclude.h"
#include "swigpyrun.h"


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

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   CReqReadKeyword request;
   request.m_keywordId = keywordId;
   sendRequest(kReqReadKeyword, request);

   CAnsReadKeyword answer;
   receiveAnswer(kAnsReadKeyword, answer);

   return answer.m_returnValue;
}

std::string CYScriptApiImplementation::waitForAcquisition(int keywordId, const std::string& timeout) const
{
   CReqWaitForAcquisition request;
   request.m_keywordId = keywordId;
   request.m_timeout = timeout;
   sendRequest(kReqWaitForAcquisition, request);

   CAnsWaitForAcquisition answer;
   if (!tryReceiveAnswer(kAnsWaitForAcquisition, answer, timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout)))
      return std::string();

   return answer.m_returnValue;
}

std::pair<int, std::string> CYScriptApiImplementation::waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const
{
   CReqWaitForAcquisitions request;
   request.m_keywordIdList = keywordIdList;
   request.m_timeout = timeout;
   sendRequest(kReqWaitForAcquisitions, request);

   CAnsWaitForAcquisitions answer;
   if (!tryReceiveAnswer(kAnsWaitForAcquisitions, answer, timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout)))
      return std::pair<int, std::string>();

   return answer.m_returnValue;
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   CReqWriteKeyword request;
   request.m_keywordId = keywordId;
   request.m_newState = newState;
   sendRequest(kReqWriteKeyword, request);
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   CReqSendNotification request;
   request.m_keywordId = keywordId;
   request.m_recipientId = recipientId;
   request.m_message = message;
   sendRequest(kReqSendNotification, request);
}

std::string CYScriptApiImplementation::getInfo(const std::string& key) const
{
   CReqGetInfo request;
   request.m_key = key;
   sendRequest(kReqGetInfo, request);

   CAnsGetInfo answer;
   receiveAnswer(kAnsGetInfo, answer);

   return answer.m_returnValue;
}

