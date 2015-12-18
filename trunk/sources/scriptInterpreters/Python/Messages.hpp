#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>

const size_t m_messageQueueMessageSize(1000);


//--------------------------------------------------------------
/// \brief Messages requests identifiers
//--------------------------------------------------------------
enum ERequestIdentifier
{
   kReqGetKeywordId = 0,
   kReqGetRecipientId,
   kReqReadKeyword,
   kReqWaitForNextAcquisition,
   kReqWaitForNextAcquisitions,
   kReqWaitForEvent,
   kReqWait,
   kReqWriteKeyword,
   kReqSendNotification,
   kReqGetInfo,
};

//--------------------------------------------------------------
/// \brief Messages answers identifiers
//--------------------------------------------------------------
enum EAnswerIdentifier
{
   kAnsGetKeywordId = 0,
   kAnsGetRecipientId,
   kAnsReadKeyword,
   kAnsWaitForAcquisition,
   kAnsWaitForAcquisitions,
   kAnsWaitForEvent,
   kAnsWait,
   kAnsWriteKeyword,
   kAnsSendNotification,
   kAnsGetInfo,
};
