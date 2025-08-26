#pragma once
#include "IMessageHandler.h"
#include "../response/ReceivedMessage.h"

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack enable post master command
    //--------------------------------------------------------------
    class CEnablePostMasterCommand final
    {
    public:
        explicit CEnablePostMasterCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CEnablePostMasterCommand() = default;

        /// \brief Send request and check answer
        /// \param mailboxesNumber 0-255 But seems that 20 is the max for a standard controller. Set to 0 to disable the controller postmaster function.
        void sendAndReceive(unsigned char mailboxesNumber) const;

    private:
        static void processAnswer(response::CReceivedMessage::EReturnCode returnCode,
                                  const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;
    };
} // namespace message
