#include "stdafx.h"
#include "ReceivedMessage.h"

#include "../../ProtocolException.hpp"

using namespace message::response;

CReceivedMessage::CReceivedMessage(const boost::shared_ptr<const CEsp3ReceivedPacket>& esp3Packet)
    : m_returnCode(static_cast<EReturnCode>(esp3Packet->data()[0])),
      m_responseData(esp3Packet->data().begin() + 1, esp3Packet->data().end()),
      m_responseOtionalData(esp3Packet->optional().begin(), esp3Packet->optional().end())
{
    if (m_returnCode != RET_OK)
        return;

    if (esp3Packet->header().dataLength() != m_responseData.size() + 1)
        throw CProtocolException(
            (boost::format("Invalid data length %1%, %2% in header")
                % (m_responseData.size() + 1)
                % esp3Packet->header().dataLength()).str());

    if (esp3Packet->header().optionalLength() != m_responseOtionalData.size())
        throw CProtocolException(
            (boost::format("Invalid optional data length %1%, %2% in header")
                % m_responseOtionalData.size()
                % esp3Packet->header().optionalLength()).str());
}

CReceivedMessage::EReturnCode CReceivedMessage::returnCode() const
{
    return m_returnCode;
}

const std::vector<unsigned char>& CReceivedMessage::responseData() const
{
    return m_responseData;
}

const std::vector<unsigned char>& CReceivedMessage::responseOptionalData() const
{
    return m_responseOtionalData;
}

std::string CReceivedMessage::toString(const EReturnCode returnCode)
{
    switch (returnCode)
    {
    case RET_OK:
        return "RET_OK";
    case RET_ERROR:
        return "RET_ERROR";
    case RET_NOT_SUPPORTED:
        return "RET_NOT_SUPPORTED";
    case RET_WRONG_PARAM:
        return "RET_WRONG_PARAM";
    case RET_OPERATION_DENIED:
        return "RET_OPERATION_DENIED";
    case RET_LOCK_SET:
        return "RET_LOCK_SET";
    case RET_BUFFER_TO_SMALL:
        return "RET_BUFFER_TO_SMALL";
    case RET_NO_FREE_BUFFER:
        return "RET_NO_FREE_BUFFER";
    case RET_SEPCIAL_RC_OFFSET:
        return "RET_SEPCIAL_RC_OFFSET";
    default:  // NOLINT(clang-diagnostic-covered-switch-default)
        return (boost::format("Unexpected return code %1%") % returnCode).str();
    }
}
