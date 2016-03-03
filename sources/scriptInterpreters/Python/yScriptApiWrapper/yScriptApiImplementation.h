#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>
#include "../Messages.h"


//-----------------------------------------------------
///\brief The Yadoms script API implementation for the Python wrapper
//-----------------------------------------------------
class CYScriptApiImplementation : public shared::script::yScriptApi::IYScriptApi
{
public:
   //-----------------------------------------------------
   ///\brief               Constructor
   ///\param[in] yScriptApiAccessorId  The context accessor ID provided by Yadoms
   //-----------------------------------------------------
   CYScriptApiImplementation(const std::string& yScriptApiAccessorId);

   //-----------------------------------------------------
   ///\brief               Destructor
   //-----------------------------------------------------
   virtual ~CYScriptApiImplementation();

   // shared::script::yScriptApi::IYScriptApi implementation
   virtual int getKeywordId(const std::string& deviceName, const std::string& keywordName) const;
   virtual int getRecipientId(const std::string& firstName, const std::string& lastName) const;
   virtual std::string readKeyword(int keywordId) const;
   virtual std::string waitForNextAcquisition(int keywordId, const std::string& timeout = std::string()) const;
   virtual std::pair<int, std::string> waitForNextAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const;
   virtual shared::script::yScriptApi::CWaitForEventResult waitForEvent(const std::vector<int> keywordIdList, bool receiveDateTimeEvent, const std::string& timeout = std::string()) const;
   virtual void writeKeyword(int keywordId, const std::string& newState);
   virtual void sendNotification(int keywordId, int recipientId, const std::string& message);
   virtual std::string getInfo(EInfoKeys key) const;
   // [END] shared::script::yScriptApi::IYScriptApi implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Send a request
   /// \param[in] request Request to send
   //--------------------------------------------------------------
   void sendRequest(const pbRequest::msg& request) const;

   //--------------------------------------------------------------
   /// \brief	Wait for an answer
   /// \param[in] answer Received answer from Yadoms
   /// \throw std::runtime_error if message queue error
   /// \throw shared::exception::CInvalidParameter if error parsing message
   //--------------------------------------------------------------
   void receiveAnswer(pbAnswer::msg& answer) const;

private:
   //-----------------------------------------------------
   ///\brief               The message queues used to exchange data with Yadoms
   //\details These objects must be mutable to preserve IYScriptApi const interface
   //-----------------------------------------------------
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_receiveMessageQueue;

   //-----------------------------------------------------
   ///\brief               The message queue buffer, localy used but defined here to be allocated only once
   //-----------------------------------------------------
   mutable unsigned char m_mqBuffer[m_messageQueueMessageSize];
};
