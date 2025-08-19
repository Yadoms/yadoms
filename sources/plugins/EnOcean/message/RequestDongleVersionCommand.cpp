#include "stdafx.h"
#include "RequestDongleVersionCommand.h"

#include <ProtocolException.hpp>

#include "CommonCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CRequestDongleVersionCommand::Version::Version(unsigned int main,
                                                   unsigned int beta,
                                                   unsigned int alpha,
                                                   unsigned int build)
        : m_main(main),
          m_beta(beta),
          m_alpha(alpha),
          m_build(build)
    {
    }

    std::string CRequestDongleVersionCommand::Version::toString() const
    {
        std::ostringstream str;
        str << m_main << '.' << m_beta << '.' << m_alpha << '.' << m_build;
        return str.str();
    }

    CRequestDongleVersionCommand::CRequestDongleVersionCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CRequestDongleVersionCommand::sendAndReceive()
    {
        YADOMS_LOG(information) << "Read dongle version...";

        CCommonCommandSendMessage sendMessage(CCommonCommandSendMessage::CO_RD_VERSION);

        boost::shared_ptr<const CEsp3ReceivedPacket> answer;
        if (!m_messageHandler->send(sendMessage,
                                    [](const boost::shared_ptr<const CEsp3ReceivedPacket>& esp3Packet)
                                    {
                                        if (esp3Packet->header().packetType() == RESPONSE)
                                            return true;
                                        YADOMS_LOG(warning) << "Unexpected message received : wrong packet type : " << esp3Packet->header().
                                            packetType();
                                        return false;
                                    },
                                    [&](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                                    {
                                        answer = std::move(esp3Packet);
                                    }))
            throw CProtocolException("Timeout waiting answer");

        static constexpr auto ExpectedDataSize = 33u;
        if (answer->header().dataLength() != ExpectedDataSize)
            throw CProtocolException(
                (boost::format("Invalid data length %1%, expected %2%")
                    % answer->header().dataLength()
                    % ExpectedDataSize).str());

        processAnswer(CResponseReceivedMessage(answer),
                      "CO_RD_VERSION");
    }

    CRequestDongleVersionCommand::Version CRequestDongleVersionCommand::appVersion() const
    {
        return m_appVersion;
    }

    CRequestDongleVersionCommand::Version CRequestDongleVersionCommand::apiVersion() const
    {
        return m_apiVersion;
    }

    std::string CRequestDongleVersionCommand::chipId() const
    {
        return m_chipId;
    }

    unsigned int CRequestDongleVersionCommand::chipVersion() const
    {
        return m_chipVersion;
    }

    std::string CRequestDongleVersionCommand::appDescription() const
    {
        return m_appDescription;
    }

    std::string CRequestDongleVersionCommand::fullVersion() const
    {
        std::ostringstream ss;
        ss << "EnOcean dongle Version " << m_appVersion.toString() <<
            " " << m_appDescription << "" <<
            ", api " << m_apiVersion.toString() <<
            ", chipId " << m_chipId <<
            ", chipVersion " << std::hex << m_chipVersion;
        return ss.str();
    }

    void CRequestDongleVersionCommand::processAnswer(const CResponseReceivedMessage& response,
                                                     const std::string& requestName)
    {
        if (response.returnCode() != CResponseReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + CResponseReceivedMessage::toString(response.returnCode()));

        m_appVersion = Version(response.responseData()[0],
                               response.responseData()[1],
                               response.responseData()[2],
                               response.responseData()[3]);

        m_apiVersion = Version(response.responseData()[4],
                               response.responseData()[5],
                               response.responseData()[6],
                               response.responseData()[7]);

        m_chipId = deviceIdToString(response.responseData()[8] << 24
            | response.responseData()[9] << 16
            | response.responseData()[10] << 8
            | response.responseData()[11]);

        m_chipVersion = response.responseData()[12] << 24
            | response.responseData()[13] << 16
            | response.responseData()[14] << 8
            | response.responseData()[15];

        m_appDescription = std::string(response.responseData().begin() + 16,
                                       response.responseData().end());
        m_appDescription.erase(m_appDescription.find_last_not_of('\0') + 1);

        YADOMS_LOG(information) << "  ==> " << fullVersion();
    }
} // namespace message
