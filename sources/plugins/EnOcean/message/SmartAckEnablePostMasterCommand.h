#pragma once
#include "IMessageHandler.h"
#include "ResponseReceivedMessage.h"

namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack enable post master command
    //--------------------------------------------------------------
    class CSmartAckEnablePostMasterCommand final
    {
    public:
        explicit CSmartAckEnablePostMasterCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CSmartAckEnablePostMasterCommand() = default;

        /// \brief Send request and check answer
        /// \param mailboxesNumber 0-255 But seems that 20 is the max for a standard controller. Set to 0 to disable the controller postmaster function.
        void sendAndReceive(unsigned char mailboxesNumber) const;

    private:
        static void processAnswer(CResponseReceivedMessage::EReturnCode returnCode,
                                  const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;
    };
} // namespace message
