#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/DataContainer.h>
#include "../Messages.hpp"

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
   void sendRequest(const google::protobuf::Message& request) const;
   void sendRequest(ERequestIdentifier requestId, const shared::CDataContainer& request) const;//TODO virer

   //--------------------------------------------------------------
   /// \brief	Wait for an answer
   /// \param[in] expectedAnswerId The expected answer message identifier
   /// \return The answer
   /// \throw std::runtime_error if message queue error
   /// \throw std::out_of_range if wrong answer message identifier received
   //--------------------------------------------------------------
   shared::CDataContainer receiveAnswer(EAnswerIdentifier expectedAnswerId) const;

private:
   //-----------------------------------------------------
   ///\brief               The message queues used to exchange data with Yadoms
   //\details These objects must be mutable to preserve IYScriptApi const interface
   //-----------------------------------------------------
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_receiveMessageQueue;
};
