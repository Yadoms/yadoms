#pragma once
#include "IEnOceanReceiveThread.h"
#include <shared/event/EventHandler.hpp>

//--------------------------------------------------------------
/// \brief	Implementation of receiver thread
//--------------------------------------------------------------
class CEnOceanReceiveThread : public IEnOceanReceiveThread
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CEnOceanReceiveThread(shared::event::CEventHandler& mainEventHandler,
                         int mainEvtPortConnection,
                         int mainEvtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CEnOceanReceiveThread();

   // IEnOceanReceiveThread implementation
   void start() override;
   void stop() override;
   shared::event::CEventHandler& getEventHandler() override;
   int getEvtPortConnection() override;
   int getEvtPortDataReceived() override;
   void setHook(boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) override;
   bool waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout) override;
   // [END] IEnOceanReceiveThread implementation

protected:
   void doWork();

   void onDataReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> message);

private:
   shared::event::CEventHandler& m_mainEventHandler;
   int m_mainEvtPortConnection;
   int m_mainEvtPortDataReceived;
   boost::thread m_thread;
   shared::event::CEventHandler m_eventHandler;

   // Hook
   mutable boost::recursive_mutex m_hookMutex;
   boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> m_isExpectedMessageHookFct;
   boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> m_onReceiveHookFct;
   shared::event::CEventHandler m_waitAnswerEventHandler;
};

