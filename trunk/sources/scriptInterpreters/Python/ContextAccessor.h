#pragma once
#include "IContextAccessor.h"
#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/ThreadBase.h>
#include <shared/DataContainer.h>
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
   void processMessage(const char* message, size_t messageSize, boost::interprocess::message_queue& messageQueue);

   //--------------------------------------------------------------
   /// \brief	Process messages
   /// \param[in] request Received requests
   /// \param[in] messageQueue Message Queue used for answer
   //--------------------------------------------------------------
   void processGetKeywordId            (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processGetRecipientId          (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processReadKeyword             (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processWaitForNextAcquisition  (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processWaitForNextAcquisitions (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processWaitForEvent            (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processWriteKeyword            (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processSendNotification        (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);
   void processGetInfo                 (const shared::CDataContainer& request, boost::interprocess::message_queue& messageQueue);

   //--------------------------------------------------------------
   /// \brief	Send an answer
   /// \param[in] answerId The answer message identifier
   /// \param[in] answer The answer
   /// \param[in] boost::interprocess::message_queue Message queue used to send answer
   //--------------------------------------------------------------
   void sendAnswer(EAnswerIdentifier answerId, const shared::CDataContainer& answer, boost::interprocess::message_queue& messageQueue);

   //--------------------------------------------------------------
   /// \brief	   Wait that context is ready
   //--------------------------------------------------------------
   void waitForReady();

   //--------------------------------------------------------------
   /// \brief	   Set context as ready
   //--------------------------------------------------------------
   void setReady();

private:
   //--------------------------------------------------------------
   /// \brief	Message queue max message number
   //--------------------------------------------------------------
   static const size_t m_maxMessages;

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



