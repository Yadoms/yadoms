#pragma once
#include <string>

namespace message
{
    //--------------------------------------------------------------
    /// \brief	Smart ack client
    //--------------------------------------------------------------
    class CSmartAckClient final
    {
    public:
        CSmartAckClient(const std::string& id,
                        const std::string& controllerId,
                        const unsigned int mailboxIndex);

        std::string id() const;
        std::string controllerId() const;
        unsigned int mailboxIndex() const;

    private:
        std::string m_id;
        std::string m_controllerId;
        unsigned int m_mailboxIndex;
    };
} // namespace message
