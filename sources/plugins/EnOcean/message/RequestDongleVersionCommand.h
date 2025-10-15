#pragma once
#include "IMessageHandler.h"
#include "response/ReceivedMessage.h"

namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Request dongle version command
    //--------------------------------------------------------------
    class CRequestDongleVersionCommand final
    {
    public:
        class Version final
        {
        public:
            Version(unsigned int main,
                    unsigned int beta,
                    unsigned int alpha,
                    unsigned int build);
            ~Version() = default;

            std::string toString() const;

        private:
            unsigned int m_main;
            unsigned int m_beta;
            unsigned int m_alpha;
            unsigned int m_build;
        };

        explicit CRequestDongleVersionCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CRequestDongleVersionCommand() = default;

        void sendAndReceive();

        [[nodiscard]] Version appVersion() const;
        [[nodiscard]] Version apiVersion() const;
        [[nodiscard]] std::string chipId() const;
        [[nodiscard]] unsigned int chipVersion() const;
        [[nodiscard]] std::string appDescription() const;
        [[nodiscard]] std::string fullVersion() const;

    private:
        void processAnswer(const response::CReceivedMessage& response,
                           const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;

        Version m_appVersion{0, 0, 0, 0};
        Version m_apiVersion{0, 0, 0, 0};
        std::string m_chipId;
        unsigned int m_chipVersion = 0;
        std::string m_appDescription;
    };
} // namespace message
