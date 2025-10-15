#pragma once
#include "../Esp3SendPacket.h"

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	EnOcean send smart ack command message
    //--------------------------------------------------------------
    class CCommandSendMessage final : public CEsp3SendPacket
    {
    public:
        enum ESmartAckCommand
        {
            SA_WR_LEARNMODE = 1, //Set/Reset Smart Ack learn mode
            SA_RD_LEARNMODE = 2, //Get Smart Ack learn mode state
            SA_WR_LEARNCONFIRM = 3, //Used for Smart Ack to add or delete a mailbox of a client
            SA_WR_CLIENTLEARNRQ = 4, //Send Smart Ack Learn request (Client)
            SA_WR_RESET = 5, //Send reset command to a Smart Ack client
            SA_RD_LEARNEDCLIENTS = 6, //Get Smart Ack learned sensors / mailboxes
            SA_WR_RECLAIMS = 7, //Set number of reclaim attempts
            SA_WR_POSTMASTER = 8, //Activate/Deactivate Postmaster functionality
            SA_RD_MAILBOX = 9, //Read mailbox status for a SmartAck client
            SA_DEL_MAILBOX = 10, //Delete mailbox for a SmartAck client
        };

        explicit CCommandSendMessage(ESmartAckCommand command,
                                     const std::vector<unsigned char>& data = {});
        ~CCommandSendMessage() override = default;

        boost::shared_ptr<const std::vector<unsigned char>> buffer() override;

    private:
        ESmartAckCommand m_command;
        const std::vector<unsigned char> m_data;
    };
} // namespace message
