#pragma once
#include <shared/communication/IAsyncPort.h>
#include "IMessageHandler.h"

//--------------------------------------------------------------
/// \brief	Implementation of message handler
//--------------------------------------------------------------
class CMessageHandler : public IMessageHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                  Constructor
   /// \param[in] port           The async port in use
   /// \param[in] port           The event handler when a message is received and not hooked
   /// \param[in] port           The event handler id
   //--------------------------------------------------------------
   CMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port, shared::event::CEventHandler& mainEventHandler, int mainEvtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                  Destructor
   //--------------------------------------------------------------
   virtual ~CMessageHandler();

   // IMessageHandler implementation
   void onReceived(boost::shared_ptr<const frames::CFrame> receivedMessage);
   bool send(const std::string & sendMessage, boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct);
   void send(const std::string& sendMessage);
   bool sendFile(const std::string & sendMessage, boost::function<void(float, const std::string &)> onProgressHandler);
   // [END] IMessageHandler implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                           Configure hook
   /// \param [in] isExpectedMessageFct   Lambda to identify the expected message. Must return true if received message is the expected one. (called by different thread).
   /// \param [in] onReceiveFct           Lambda to process received message. (called by different thread).
   //--------------------------------------------------------------
   void setHook(boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct);

   //--------------------------------------------------------------
   /// \brief	                           Wait for an answer of dongle
   /// \param [in] answerTimeout          The answer timeout
   /// \return                            true if answer received before timeout
   //--------------------------------------------------------------
   bool waitAnswer(const boost::posix_time::time_duration& answerTimeout);

private:
   //--------------------------------------------------------------
   /// \brief	                  The async port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief	                  The event handler
   //--------------------------------------------------------------
   shared::event::CEventHandler& m_mainEventHandler;

   //--------------------------------------------------------------
   /// \brief	                  The event id
   //--------------------------------------------------------------
   int m_mainEvtPortDataReceived;

   //--------------------------------------------------------------
   /// \brief	                  The hook mutex
   //--------------------------------------------------------------
   mutable boost::recursive_mutex m_hookMutex;

   //--------------------------------------------------------------
   /// \brief	                  Lambda to identify the expected message. Must return true if received message is the expected one. (called by different thread).
   //--------------------------------------------------------------
   boost::function<bool(boost::shared_ptr<const frames::CFrame>)> m_isExpectedMessageHookFct;

   //--------------------------------------------------------------
   /// \brief	                  Lambda to process received message. (called by different thread).
   //--------------------------------------------------------------
   boost::function<void(boost::shared_ptr<const frames::CFrame>)> m_onReceiveHookFct;

   //--------------------------------------------------------------
   /// \brief	                  The wait event handler (to manage tiemout)
   //--------------------------------------------------------------
   shared::event::CEventHandler m_waitAnswerEventHandler;
};

