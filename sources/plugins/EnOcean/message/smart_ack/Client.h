#pragma once
#include <string>

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	Smart ack client
    //--------------------------------------------------------------
    class CClient final
    {
    public:
        CClient(const std::string& id,
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
