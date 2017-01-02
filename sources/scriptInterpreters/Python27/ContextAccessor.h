#pragma once
#include "IContextAccessor.h"
#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/ThreadBase.h>
#include "Messages.h"


//--------------------------------------------------------------
/// \brief	yScriptApi context accessor, based on message queues
//--------------------------------------------------------------
class CContextAccessor : public IContextAccessor,
                         protected shared::CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] yScriptApi IYScriptApi context instance
   //--------------------------------------------------------------
   explicit CContextAccessor(boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CContextAccessor();

   // IContextAccessor Implementation
   std::string id() const override;
   // [END] IContextAccessor Implementation

protected:
   // CThreadBase Implementation
   void doWork() override;
   // [END] CThreadBase Implementation

   //--------------------------------------------------------------
   /// \brief	Create a unique context accessor ID (unique on full system)
   //--------------------------------------------------------------
   static std::string createId();

   //--------------------------------------------------------------
   /// \brief	Process a received message
   /// \param[in] message The message data
   /// \param[in] messageSize The message size
   /// \param[in] messageQueue Message queue used to send answer
   //--------------------------------------------------------------
   void processMessage(const void* message,
                       size_t messageSize,
                       boost::interprocess::message_queue& messageQueue);

   //--------------------------------------------------------------
   /// \brief	Process messages
   /// \param[in] request Received requests
   /// \param[in] messageQueue Message Queue used for answer
   //--------------------------------------------------------------
   void processGetKeywordId(const pbRequest::GetKeywordId& request,
                            boost::interprocess::message_queue& messageQueue);
   void processGetRecipientId(const pbRequest::GetRecipientId& request,
                              boost::interprocess::message_queue& messageQueue);
   void processReadKeyword(const pbRequest::ReadKeyword& request,
                           boost::interprocess::message_queue& messageQueue);
   void processWaitForNextAcquisition(const pbRequest::WaitForNextAcquisition& request,
                                      boost::interprocess::message_queue& messageQueue);
   void processWaitForNextAcquisitions(const pbRequest::WaitForNextAcquisitions& request,
                                       boost::interprocess::message_queue& messageQueue);
   void processWaitForEvent(const pbRequest::WaitForEvent& request,
                            boost::interprocess::message_queue& messageQueue);
   void processWriteKeyword(const pbRequest::WriteKeyword& request,
                            boost::interprocess::message_queue& messageQueue);
   void processSendNotification(const pbRequest::SendNotification& request,
                                boost::interprocess::message_queue& messageQueue);
   void processGetInfo(const pbRequest::GetInfo& request,
                       boost::interprocess::message_queue& messageQueue);
   void processGetKeywordsByCapacity(const pbRequest::GetKeywordsByCapacity& request,
                                     boost::interprocess::message_queue& messageQueue);
   void processGetKeywordName(const pbRequest::GetKeywordName& request,
                              boost::interprocess::message_queue& messageQueue);
   void processGetKeywordDeviceName(const pbRequest::GetKeywordDeviceName& request,
                                    boost::interprocess::message_queue& messageQueue);
   //--------------------------------------------------------------
   /// \brief	Send an answer
   /// \param[in] answer The answer
   /// \param[in] messageQueue Message queue used to send answer
   //--------------------------------------------------------------
   void sendAnswer(const pbAnswer::msg& answer,
                   boost::interprocess::message_queue& messageQueue);

private:
   //--------------------------------------------------------------
   /// \brief	Message queue max message number
   //--------------------------------------------------------------
   static const size_t m_maxMessages;

   //-----------------------------------------------------
   ///\brief               The message queue buffer, localy used but defined here to be allocated only once
   //-----------------------------------------------------
   unsigned char m_mqBuffer[m_messageQueueMessageSize];

   //-----------------------------------------------------
   ///\brief               The send mutex. Protect m_mqBuffer and m_sendMessageQueue
   //-----------------------------------------------------
   mutable boost::recursive_mutex m_sendMutex;

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
