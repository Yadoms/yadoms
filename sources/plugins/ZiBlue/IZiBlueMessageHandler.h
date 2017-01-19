#pragma once
#include "frames/Frame.h"

//--------------------------------------------------------------
/// \brief	Implementation of message handler
//--------------------------------------------------------------
class IZiBlueMessageHandler
{
public:
   virtual ~IZiBlueMessageHandler() {}

   virtual void onReceived(boost::shared_ptr<const frames::CFrame> receivedMessage) = 0;
   virtual bool send(const std::string & sendMessage, boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct) = 0;
   virtual void send(const std::string& sendMessage) = 0;
};

