#pragma once
#include "ResponseReceivedMessage.h"


namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean dongle Smart Ack learn mode RESPONSE message
    ///
    /// This class manages a Smart Ack learn mode RESPONSE EnOcean message.
    //--------------------------------------------------------------
    class CSmartAckLearnModeResponseReceivedMessage final
    {
    public:
        enum class LearnModeExtended
        {
            SimpleLearnMode,
            AdvancedLearnMode,
            AdvancedLearnModeSelectRepeater,
        };

        //--------------------------------------------------------------
        /// \brief	                           Constructor
        /// \param[in] response                The response message
        //--------------------------------------------------------------
        explicit CSmartAckLearnModeResponseReceivedMessage(const boost::shared_ptr<const CResponseReceivedMessage>& response);
        ~CSmartAckLearnModeResponseReceivedMessage() = default;

        bool learnModeEnable() const;
        LearnModeExtended learnModeExtended() const;
        static std::string toString(LearnModeExtended learnModeExtended);

    private:
        static LearnModeExtended toLearnModeExtended(unsigned char byte);

        bool m_learnModeEnable;
        LearnModeExtended m_learnModeExtended;
    };
} // namespace message
