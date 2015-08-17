#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>
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
   virtual std::string readKeyword(int keywordId) const;
   virtual std::string waitForAcquisition(int keywordId, const std::string& timeout = std::string()) const;
   virtual std::pair<int, std::string> waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const;
   virtual void writeKeyword(int keywordId, const std::string& newState);
   virtual void sendNotification(int keywordId, int recipientId, const std::string& message);
   virtual std::string getInfo(const std::string& key) const;
   // [END] shared::script::yScriptApi::IYScriptApi implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Send a request
   /// \template RequestType The type of the request
   /// \param[in] requestId The request message identifier
   /// \param[in] request The request
   //--------------------------------------------------------------
   template <class RequestType>
   void sendRequest(ERequestIdentifier requestId, const RequestType& request) const
   {
      try
      {
         std::ostringstream oss(std::ios::binary);
         boost::archive::binary_oarchive oa(oss);
         oa << requestId;
         oa << request;
         if (oss.str().size() > m_messageQueueMessageSize)
            throw std::overflow_error("yScriptApiWrapper::sendRequest : request is too big");
         m_sendMessageQueue->send(oss.str().c_str(), oss.str().size(), 0);
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         throw std::overflow_error(std::string("yScriptApiWrapper::sendRequest : Error at IYScriptApi method call, ") + ex.what());
      }
   }

   //--------------------------------------------------------------
   /// \brief	Wait for an answer
   /// \template AnswerType The type of the answer to unserialize
   /// \param[in] expectedAnswerId The expected answer message identifier
   /// \param[out] answer The unserialized answer
   /// \param[in] timeout Duration to wait.
   /// \throw std::runtime_error if message queue error
   /// \throw std::out_of_range if wrong answer message identifier received
   //--------------------------------------------------------------
   template <class AnswerType>
   void receiveAnswer(EAnswerIdentifier expectedAnswerId, AnswerType& answer, const boost::posix_time::time_duration& timeout = boost::posix_time::time_duration()) const
   {
      if (!tryReceiveAnswer(expectedAnswerId, answer, timeout))
         throw std::runtime_error("yScriptApiWrapper::receiveAnswer : Timeout waiting for Yadoms answer");
   }

   //--------------------------------------------------------------
   /// \brief	Try to read an answer
   /// \template AnswerType The type of the answer to unserialize
   /// \param[in] expectedAnswerId The expected answer message identifier
   /// \param[out] answer The unserialized answer
   /// \param[in] timeout Duration to wait.
   /// return true if answer received, false if timeout
   /// \throw std::runtime_error if message queue error
   /// \throw std::out_of_range if wrong answer message identifier received
   //--------------------------------------------------------------
   template <class AnswerType>
   bool tryReceiveAnswer(EAnswerIdentifier expectedAnswerId, AnswerType& answer, const boost::posix_time::time_duration& timeout) const
   {
      // Wait answer
      char message[m_messageQueueMessageSize];
      size_t messageSize;
      unsigned int messagePriority;

      // Add a small timeout to let time to Yadoms to answer
      if (!m_receiveMessageQueue->timed_receive(message, m_messageQueueMessageSize, messageSize, messagePriority, boost::posix_time::second_clock::universal_time() + boost::posix_time::seconds(10) + timeout))
         return false;

      if (messageSize < 1)
         throw std::runtime_error("yScriptApiWrapper::tryReceiveAnswer : received Yadoms answer is zero length");

      // Unserialize received message
      std::istringstream iss(std::string(message, messageSize), std::ios::binary);
      boost::archive::binary_iarchive ia(iss);
      EAnswerIdentifier requestId;
      ia >> requestId;

      if (requestId != expectedAnswerId)
         throw std::out_of_range("yScriptApiWrapper::tryReceiveAnswer : received Yadoms answer is wrong type");

      ia >> answer;

      return true;
   }

private:
   //-----------------------------------------------------
   ///\brief               The message queues used to exchange data with Yadoms
   //\details These objects must be mutable to preserve IYScriptApi const interface
   //-----------------------------------------------------
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;
   mutable boost::shared_ptr<boost::interprocess::message_queue> m_receiveMessageQueue;
};
