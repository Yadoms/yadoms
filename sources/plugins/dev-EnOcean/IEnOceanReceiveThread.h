#pragma once
#include "message/ReceivedMessage.h"

//--------------------------------------------------------------
/// \brief	Interface of receiver thread
//--------------------------------------------------------------
class IEnOceanReceiveThread
{
public:
   virtual ~IEnOceanReceiveThread()
   {
   }

   virtual void start() = 0;
   virtual void stop() = 0;

   virtual shared::event::CEventHandler& getEventHandler() = 0;
   virtual int getEvtPortConnection() = 0;
   virtual int getEvtPortDataReceived() = 0;
   virtual void setHook(boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                        boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) = 0;
   virtual bool waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout) = 0;
};

