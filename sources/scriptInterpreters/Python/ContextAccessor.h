#pragma once
#include "IContextAccessor.h"
#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/ThreadBase.h>
#include "Messages.hpp"


//--------------------------------------------------------------
/// \brief	yScriptApi context accessor, based on message queues
//--------------------------------------------------------------
class CContextAccessor : public IContextAccessor, protected shared::CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] context IYScriptApi context instance
   //--------------------------------------------------------------
   CContextAccessor(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CContextAccessor();

protected:
   // IContextAccessor Implementation
   virtual std::string id() const;
   // [END] IContextAccessor Implementation

   // CThreadBase Implementation
   virtual void doWork();
   // [END] CThreadBase Implementation

   //--------------------------------------------------------------
   /// \brief	Create a unique context accessor ID (unique on full system)
   //--------------------------------------------------------------
   static std::string createId();

   //--------------------------------------------------------------
   /// \brief	Process a received message
   /// \param[in] message The message data
   /// \param[in] messageSize The message size
   /// \param[in] boost::interprocess::message_queue Message queue used to send answer
   //--------------------------------------------------------------
   void processMessage(const void* message, size_t messageSize, boost::interprocess::message_queue& messageQueue);

   //--------------------------------------------------------------
   /// \brief	Process messages
   /// \param[in] request Received requests
   /// \param[in] messageQueue Message Queue used for answer
   //--------------------------------------------------------------
   void processGetKeywordId(const protobufMessage::Request_GetKeywordId& request, boost::interprocess::message_queue& messageQueue);
   void processGetRecipientId(const protobufMessage::Request_GetRecipientId& request, boost::interprocess::message_queue& messageQueue);
   void processReadKeyword(const protobufMessage::Request_ReadKeyword& request, boost::interprocess::message_queue& messageQueue);
   void processWaitForNextAcquisition(const protobufMessage::Request_WaitForNextAcquisition& request, boost::interprocess::message_queue& messageQueue);
   void processWaitForNextAcquisitions(const protobufMessage::Request_WaitForNextAcquisitions& request, boost::interprocess::message_queue& messageQueue);
   void processWaitForEvent(const protobufMessage::Request_WaitForEvent& request, boost::interprocess::message_queue& messageQueue);
   void processWriteKeyword(const protobufMessage::Request_WriteKeyword& request, boost::interprocess::message_queue& messageQueue);
   void processSendNotification(const protobufMessage::Request_SendNotification& request, boost::interprocess::message_queue& messageQueue);
   void processGetInfo(const protobufMessage::Request_GetInfo& request, boost::interprocess::message_queue& messageQueue);

   //--------------------------------------------------------------
   /// \brief	Send an answer
   /// \param[in] answer The answer
   /// \param[in] boost::interprocess::message_queue Message queue used to send answer
   //--------------------------------------------------------------
   void sendAnswer(const protobufMessage::Answer& answer, boost::interprocess::message_queue& messageQueue);

private:
   //--------------------------------------------------------------
   /// \brief	Message queue max message number
   //--------------------------------------------------------------
   static const size_t m_maxMessages;

   //-----------------------------------------------------
   ///\brief               The message queue buffer, localy used but defined here to be allocated only once
   //-----------------------------------------------------
   unsigned char m_mqBuffer[m_messageQueueMessageSize];

   //--------------------------------------------------------------
   /// \brief	IYScriptApi context instance
   //--------------------------------------------------------------
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> m_scriptApi;

   //--------------------------------------------------------------
   /// \brief	Context accessor ID (unique on full system)
   //--------------------------------------------------------------
   std::string m_id;

   //--------------------------------------------------------------
   /// \brief	   Barrier waiting that context is ready
   //--------------------------------------------------------------
   mutable boost::barrier m_readyBarrier;
};



