#pragma once
#include <shared/communication/IAsyncPort.h>
#include "IZiBlueMessageHandler.h"

//--------------------------------------------------------------
/// \brief	Implementation of message handler
//--------------------------------------------------------------
class CZiBlueMessageHandler : public IZiBlueMessageHandler
{
public:
   CZiBlueMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port, shared::event::CEventHandler& mainEventHandler, int mainEvtPortDataReceived);
   virtual ~CZiBlueMessageHandler();

   // IZiBlueMessageHandler implementation
   void onReceived(boost::shared_ptr<const frames::CFrame> receivedMessage);
   bool send(const std::string & sendMessage, boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct);
   void send(const std::string& sendMessage);
   // [END] IZiBlueMessageHandler implementation

protected:
   void setHook(boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct);
   bool waitAnswer(const boost::posix_time::time_duration& nswerTimeout);

private:
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;
   shared::event::CEventHandler& m_mainEventHandler;
   int m_mainEvtPortDataReceived;

   // Hook
   mutable boost::recursive_mutex m_hookMutex;
   boost::function<bool(boost::shared_ptr<const frames::CFrame>)> m_isExpectedMessageHookFct;
   boost::function<void(boost::shared_ptr<const frames::CFrame>)> m_onReceiveHookFct;
   shared::event::CEventHandler m_waitAnswerEventHandler;
};

