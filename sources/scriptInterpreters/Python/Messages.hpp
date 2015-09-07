#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>

const size_t m_messageQueueMessageSize(1000);


//--------------------------------------------------------------
/// \brief Messages requests identifiers
//--------------------------------------------------------------
enum ERequestIdentifier
{
   kReqReadKeyword = 0,
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
   kAnsReadKeyword = 0,
   kAnsWaitForAcquisition,
   kAnsWaitForAcquisitions,
   kAnsGetInfo,
};
