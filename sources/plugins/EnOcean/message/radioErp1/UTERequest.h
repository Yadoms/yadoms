#pragma once
#include "ReceivedMessage.h"


namespace message::radioErp1
{
    //--------------------------------------------------------------
    /// \brief	EnOcean UTE message (Universal Teach-in)
    ///
    /// This class manages a UTE EnOcean message.
    //--------------------------------------------------------------
    class CUTERequest
    {
    public:
        enum class ETeachInRequest
        {
            kTeachInRequest = 0,
            kTeachInDeletionRequest,
            kNotSpecified,
            kNotUsed
        };

        enum ECommand
        {
            kTeachInQuery = 0
        };

        //--------------------------------------------------------------
        /// \brief	                           Constructor
        /// \param[in] message                 The ERP1 radio message
        //--------------------------------------------------------------
        explicit CUTERequest(const CReceivedMessage& message);
        virtual ~CUTERequest() = default;

        virtual bool bidirectionalCommunication() const;
        virtual bool teachInResponseExpected() const;
        virtual ETeachInRequest teachInRequest() const;
        virtual unsigned char command() const;
        virtual unsigned char channelNumber() const;
        virtual unsigned int manufacturerId() const;
        virtual unsigned char type() const;
        virtual unsigned char func() const;
        virtual unsigned char rorg() const;
        virtual std::string senderId() const;

    protected:
        const CReceivedMessage& m_erp1;
    };
}
