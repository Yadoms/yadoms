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
   virtual std::string waitForAcquisition(int keywordId, const std::string& timeout = std::string()) const;
   virtual std::pair<int, std::string> waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const;
   virtual void writeKeyword(int keywordId, const std::string& newState);
   virtual void sendNotification(int keywordId, int recipientId, const std::string& message);
   virtual std::string getInfo(EInfoKeys key) const;
   virtual void ruleEnable(bool enable = true);
   // [END] shared::script::yScriptApi::IYScriptApi implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Send a request
   /// \param[in] requestId The request message identifier
   /// \param[in] request The request
   //--------------------------------------------------------------
   void sendRequest(ERequestIdentifier requestId, const shared::CDataContainer& request) const;

   //--------------------------------------------------------------
   /// \brief	Wait for an answer
   /// \param[in] expectedAnswerId The expected answer message identifier
   /// \param[out] answer The unserialized answer
   /// \param[in] timeout Duration to wait.
   /// \throw std::runtime_error if message queue error
   /// \throw std::out_of_range if wrong answer message identifier received
   //--------------------------------------------------------------
   void receiveAnswer(EAnswerIdentifier expectedAnswerId, shared::CDataContainer& answer, const boost::posix_time::time_duration& timeout = boost::posix_time::time_duration()) const;

   //--------------------------------------------------------------
   /// \brief	Try to read an answer
   /// \param[in] expectedAnswerId The expected answer message identifier
   /// \param[out] answer The unserialized answer
   /// \param[in] timeout Duration to wait.
   /// return true if answer received, false if timeout
   /// \throw std::runtime_error if message queue error
   /// \throw std::out_of_range if wrong answer message identifier received
   //--------------------------------------------------------------
   bool tryReceiveAnswer(EAnswerIdentifier expectedAnswerId, shared::CDataContainer& answer, const boost::posix_time::time_duration& timeout) const;

private:
   //-----------------------------------------------------
   ///\brief               The message queues used to exchange data with Yadoms
   //\details These objects must be mutable to preserve IYScriptApi const interface
   //-----------------------------------------------------
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_receiveMessageQueue;
};
