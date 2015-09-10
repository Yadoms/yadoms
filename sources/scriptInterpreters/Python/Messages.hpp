#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>

const size_t m_messageQueueMessageSize(1000);


//--------------------------------------------------------------
/// \brief Messages requests identifiers
//--------------------------------------------------------------
enum ERequestIdentifier
{
   kReqGetKeywordId = 0,
   kReqReadKeyword,
   kReqWaitForAcquisition,
   kReqWaitForAcquisitions,
   kReqWriteKeyword,
   kReqSendNotification,
   kReqGetInfo,
   kReqRuleEnable,
};

//--------------------------------------------------------------
/// \brief Messages answers identifiers
//--------------------------------------------------------------
enum EAnswerIdentifier
{
   kAnsGetKeywordId = 0,
   kAnsReadKeyword,
   kAnsWaitForAcquisition,
   kAnsWaitForAcquisitions,
   kAnsGetInfo,
};
